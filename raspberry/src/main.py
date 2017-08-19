#!/usr/bin/python3
import serial
import struct
import subprocess
import signal
import os

playFile = "data/rickastley.mp3"
playProc = None
def killPlay():
    global playProc
    if playProc is not None:
        pid = playProc.pid
        os.kill(pid, signal.SIGINT)
        print("Kill Music")
    playProc=None

with serial.Serial('/dev/ttyUSB0', 19200, timeout=100) as ser:
    last_val=None
    while True:
        b = ser.read()
        val = struct.unpack('b', b)[0]
        if last_val is not None and val != last_val:
            if val: # This means the lights just turned on
                print("Lights turned ON")
                killPlay()    
                playProc = subprocess.Popen(["mpg123", "-f", "20000", playFile], shell=False)
            else:
                print("Lights turned OFF")
                killPlay()

        last_val = val


        # print(val)
        # if val:
        #     print("Light: ON")
        # else:
        #     print("Light: OFF")
