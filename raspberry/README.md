# Overveiw
This repository is for code related to the OpenWERX drone competition. The goal
is for the software provided by this this repository to start at boot and
continue to run throughout the operation of the drone.


## Dependencies
* mpg123 (`sudo apt-get install mpg123`)
* [Adafruit Speaker Bonnet Software](https://learn.adafruit.com/adafruit-speaker-bonnet-for-raspberry-pi/raspberry-pi-usage)
* WiringPi Library (`git clone git://git.drogon.net/wiringPi`;Read INSTALL file; NOTE: FindWiringPi.cmake added from
[here](https://stackoverflow.com/questions/30424236/add-wiringpi-lib-to-cmake-on-raspberrypi))
* libserial ([GitHub](https://github.com/crayzeewulf/libserial); before trying to build, run `sudo apt-get install build-essential libtool autoconf automake`)

## Other Notes
* Don't forget to enable the I2C kernel module. (`sudo raspi-config` -> Interfacing Options)
