import serial
import time
import threading

ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=0.5, parity=serial.PARITY_EVEN)
print(ser.name)

stop_threads = False

import queue
rx_q = queue.Queue()
tx_q = queue.Queue()

class Packet:
    def __init__(self, data, size):
        self.data = data
        self.size = size

def rx():
    while not stop_threads:
        read = ser.read(1)
        rx_q.put(Packet(read, 1))
        print("RX", read.to_bytes(1, byteorder='big'))

def tx():
    while not stop_threads:
        write = tx_q.get()
        ser.write(write.data.to_bytes(write.size, byteorder='big'))
        ser.flush()

        print("TX", write.data.to_bytes(write.size, byteorder='big'))
        
def logic():
    while not stop_threads:
        if not rx_q.empty():
            packet = rx_q.get()
            tx_q.put(packet)

t1 = threading.Thread(target=rx)
t2 = threading.Thread(target=tx)
t3 = threading.Thread(target=logic)
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