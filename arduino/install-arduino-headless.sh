#!/bin/bash

# Pull it down, put it in place
ORIGINAL_DIR=$(pwd)
cd /tmp
wget http://downloads.arduino.cc/arduino-1.5.7-linux64.tgz
tar -zxvf arduino-1.5.7-linux64.tgz
rm -rf ~/arduino-ide
mv arduino-1.5.7 ~/arduino-ide
chmod +x ~/arduino-ide/arduino
cd $ORIGINAL_DIR

# Setup headless-starter
cp data/arduino-headless ~/arduino-ide
chmod +x ~/arduino-ide/arduino-headless

# Done
echo "Installation complete. Use '~/arduino-ide/arduino-headless --upload firmware/firmware.ino port /dev/ttyUSB*' to upload sketches"


