#!/usr/bin/python3
import serial
import struct
import subprocess
import signal
import os
import glob

# Speaker Stuff
speakerFile = "data/rickastley.mp3"
speakerProc = None

def startSpeaker():
    global speakerProc
    global speakerFile
    speakerProc = subprocess.Popen(["mpg123", "-f", "200000", speakerFile], shell=False)

def stopSpeaker():
    global speakerProc
    if speakerProc is not None:
        pid = speakerProc.pid
        os.kill(pid, signal.SIGINT)
        print("Kill Music")
    speakerProc=None

# Figure out which mode to be in
# partition2 -> speaker
# partition3 -> transmitter
# partition4 -> paper
active_module=None
with open('active_module') as f:
    active_module = f.read().strip()

# Get USB serial port
port = glob.glob("/dev/ttyUSB*")[0]
print('Opening serial port "{}"'.format(port)) 

# Run main loop. Read button inputs.
with serial.Serial(port, 19200, timeout=100) as ser:
    last_led=None
    while True:
        line = ser.readline()
        try:
            led, button1, button2 = struct.unpack('???x', line)
        except:
            print("WARNING: Failed to parse line. Ignoring.")
            continue

        # Check main input (the light sensor)
        if last_led is not None and led != last_led:
            if led: # This means the lights just turned on
                print("Lights turned ON")
                startSpeaker()
            else:
                print("Lights turned OFF")
                stopSpeaker()
        last_led = led

        # Check reset buttons
        if button1:
            if active_module == 'speaker':
                print('Module "speaker" already loaded. Doing nothing.')
            else:
                print('Booting "speaker" module.')
                os.system("sudo ./utils/reboot-to-partition 2")

        if button2:
            if active_module == 'transmitter':
                print('Module "transmitter" already loaded. Doing nothing.')
            else:
                print('Booting "transmitter" module.')
                os.system("sudo ./utils/reboot-to-partition 3")


        # print(val)
        # if val:
        #     print("Light: ON")
        # else:
        #     print("Light: OFF")
