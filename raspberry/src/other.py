#!/usr/bin/python3
import subprocess
import serial
import time
import sys

global arduino

# Find out which module is active
with open('active_module', 'r') as f:
    module_name = f.read().strip()

def send(msg):
    global arduino
    arduino.write(b'{')
    arduino.write(msg.encode('ascii'))
    arduino.write(b'}')

def blocking_recv():
    global arduino

    msg_buffer = ''
    reading = False
    while True:
        c = arduino.read()
        if c == b'{':
            reading=True
        elif c==b'}' and reading:
            return msg_buffer
        elif reading:
            msg_buffer += c.decode('ascii')

# Initialize Serial
arduino = serial.Serial('/dev/ttyUSB0', 19200, timeout=5)
time.sleep(2) #give the connection a second to settle

# First, send initialization message
send('bitches')

# Wait for response
msg = blocking_recv()
if msg != "hell yeah":
    # Something is terribly wrong! Time to end it all!
    print('Failed to initialize. Aborting...')
    exit()

# If this is the transmitter module, send a frequency to broadcast on
if module_name == 'transmitter':
    freq_str = sys.argv[1]
    send('rf,'+freq_str)

# Main Loop
last_status = None
soundplayer_proc = None
while True:
    new_status = blocking_recv()
    if last_status is not None and new_status != old_status:
        if module_name in ['speakers', 'transmitter']:
            if new_status == '1':
                print('Started sound...')
                soundplayer_proc = subprocess.Popen(["mpg123", "-f", "200000", 'data/rickastley'], shell=False)
            elif new_status == '0':
                if soundplayer_proc is not None:
                    print("Killed sound.)
                    os.kill(soundplayer_proc.pid, signal.SIGINT)
                    soundplayer_proc = None
