#!/usr/bin/python3
import serial, time

# Find out which module is active
with open('active_module', 'r') as f:
    module_name = f.read().strip()


# Initialize Serial
arduino = serial.Serial('/dev/ttyUSB0', 19200, timeout=None)
time.sleep(2) #give the connection a second to settle

arduino.write(b'{')
arduino.write(module_name.encode('ascii'))
arduino.write(b'}')

