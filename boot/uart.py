import serial
import time
import threading

ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=1, parity=serial.PARITY_EVEN)
print(ser.name)

stop_threads = False

def rx():
    while not stop_threads:
        read = ser.read(4)
        if(read): print("RX", read)

def tx():
    while not stop_threads:
        ser.write(0xDEADBEEF.to_bytes(4, byteorder='big'))
        ser.flush()
        print(0xDEADBEEF.to_bytes(4, byteorder='big'))
        time.sleep(0.2)

t1 = threading.Thread(target=rx)
t2 = threading.Thread(target=tx)
t1.start()
t2.start()

try:
    t1.join()
    t2.join()
except KeyboardInterrupt:
    stop_threads = True
    t1.join()
    t2.join()
    print("Threads terminated")