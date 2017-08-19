#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

using namespace std;

char read_serial_char(string fname){
  fstream serial(fname.c_str());
  char c;
  serial >> c;
  serial.close();
  return c;
}

int main(int argc, char* argv[]){
  // Open serial
  string fname;
  if(argc > 1){
    fname = argv[1];
  }else{
    fname = "/dev/ttyUSB0";
  }
  SerialStream serial;
  serial.Open(fname.c_str());
  serial_stream.SetBaudRate(SerialStreamBuf::BAUD_115200);

  // Verify serial is Open
  if(serial.good()){
     cout << "Successfully opened serial port." << endl;
  }else{
     cout << "Failed to open serial port \"" << fname <<"\". Aborting." << endl;
     return -1;
  }

  // Read from serial
  while( serial_stream.rdbuf()->in_avail() == 0 )
  {
    usleep(100) ;
  }

  // Keep reading data from serial port and print it to the screen.
  while( serial_stream.rdbuf()->in_avail() > 0 )
  {
      char nextByte;
      serial_stream.get(nextByte);
      cout << std::hex << static_cast<int>( nextByte ) << " " ;
      usleep(100) ;
  }

  // Exit (will never get here tho)
  return 0;
}
