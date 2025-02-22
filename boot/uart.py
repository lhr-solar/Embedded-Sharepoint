import serial
import time
import threading

from enum import Enum

class Command(Enum):
    FLASH_WRITE = 0
    FLASH_READ_SINGLE = 1
    FLASH_READ_BUF = 2
    FLASH_ERASE = 3
    FLASH_MASS_ERASE = 4
    BLDR_START_AFTER_UPDATE = 5

ser = serial.Serial("/dev/ttyACM0", 9600, timeout=3000, parity=serial.PARITY_EVEN)
print(ser.name)

def pend_for_ack(timeout=5):
    ack = None
    start_time = time.time()
    while ack != b'\x06':
        ack = ser.read(1)
        if time.time() - start_time > timeout:  # 5 seconds timeout
            print("Timeout waiting for ACK")
            return False
    return True

def init():
    print("Initializing...")
    
    # Send 0xDEADBEEF command 4 times
    for i in range(4):
        print("Sending 0xDEADBEEF...")
        ser.write(b'\xDE\xAD\xBE\xEF')

    pend_for_ack()

def send_cmd(cmd, size, addr, data):
    # Send STX
    print("Sending STX...")
    ser.write(b'\x02')

    pend_for_ack()

    # COMMAND, SIZE bytes, ADDRESS addr
    print(f"Sending {cmd.name} command...")
    print(f"\tSize: {size} bytes")
    print(f"\tAddress: 0x{addr:08X}")

    ser.write(cmd.value.to_bytes(1, byteorder='big'))
    ser.write(size.to_bytes(1, byteorder='big'))
    ser.write(addr.to_bytes(4, byteorder='big'))

    # Read ACK
    pend_for_ack()

    # Send SIZE bytes of data
    print("Sending data...")
    print(f"\tData: {[f'0x{byte:02X}' for byte in data]}")
    ser.write(data)

    # Read ACK
    pend_for_ack()

    # If command is not FLASH_READ_SINGLE and FLASH_READ_BUF, exit
    if cmd != Command.FLASH_READ_SINGLE and cmd != Command.FLASH_READ_BUF: return
    
    # Read response header
    print("Reading response header...")
    read_data_size = int.from_bytes(ser.read(1), byteorder='big')
    print(f"\tData size: {read_data_size} bytes")

    # Send ACK
    print("Sending ACK...")
    ser.write(b'\x06')

    # Read response data
    print("Reading response data...")
    response_data = ser.read(read_data_size)
    print(f"\tData: {[f'0x{byte:02X}' for byte in response_data]}")

    # Send ACK
    print("Sending ACK...")
    ser.write(b'\x06')

    return response_data

def boot():
    init()
    
    # Get data from the bin
    with open("build/stm32f446ret.bin", "rb") as bin_file:
        ten_bytes = bin_file.read(10)

    send_cmd(Command.FLASH_WRITE, 10, 0x08020000, ten_bytes)
    resp = send_cmd(Command.FLASH_READ_BUF, 10, 0x08020000, b'\x00' * 10)

    print(f"Response: {[f'0x{byte:02X}' for byte in resp]}")

    print(f"Starting application...")
    send_cmd(Command.BLDR_START_AFTER_UPDATE, 0, 0, b'\x00' * 10)

if __name__ == "__main__":
    boot()