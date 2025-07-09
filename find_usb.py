from typing import List
from serial.tools import list_ports

def get_stms() -> List[str]:
    VID=0x0483
    PID=0x374B

    device_list = list_ports.comports()
    stm_list = []
    for device in device_list:
        if device.vid != None or device.pid != None:
            if (device.vid == VID and
                device.pid == PID):
                stm_list.append(device.device)
    return stm_list
