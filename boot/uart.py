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

ser = serial.Serial("/dev/ttyACM0", 9600, timeout=0.5, parity=serial.PARITY_EVEN)
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

    # WRITE, 10 bytes, address 0x08020000
    print("Sending WRITE command...")
    print("\tSize: 10 bytes")
    print("\tAddress: 0x08020000")
    ser.write(b'\x00\x0A\x08\x02\x00\x00')

    # Read ACK
    pend_for_ack()

    # Send 10 bytes of data
    print("Sending data...")
    print("\tData: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09")
    ser.write(b'\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09')

    # Read ACK
    pend_for_ack()

boot()