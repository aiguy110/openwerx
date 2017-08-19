#!/usr/bin/python3
import serial
import struct
import os

with serial.Serial('/dev/ttyUSB0', 19200, timeout=100) as ser:
    last_val=None
    while True:
        b = ser.read()
        val = struct.unpack('b', b)[0]
        if last_val is not None and val != last_val:
            if val: # This means the lights just turned on


        # print(val)
        # if val:
        #     print("Light: ON")
        # else:
        #     print("Light: OFF")
