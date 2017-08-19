# Overveiw
This repository is for code related to the OpenWERX drone competition. The goal
is for the software provided by this this repository to start at boot and
continue to run throughout the operation of the drone.


## Dependencies
* mpg123 (`sudo apt-get install mpg123`)
* [Adafruit Speaker Bonnet Software](https://learn.adafruit.com/adafruit-speaker-bonnet-for-raspberry-pi/raspberry-pi-usage)
(Only for partition with active_module=speaker)

### Developer Notes
* The file "active_module" needs to be created to allow "onboot" to work properly. At boot, the value in "active_module"
is read, and the command `./build/<active_module>` is run.
