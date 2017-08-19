#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]){
  // Open serial
  string fname;
  if(argc > 1){
    fname = argv[1];
  }else{
    fname = "/dev/ttyUSB0";
  }
  fstream serial(fname.c_str());

  // Verify serial is Open
  if(serial.is_open()){
    cout << "Successfully opened serial port." << endl;
  }else{
    cout << "Failed to open serial port \"" << fname <<"\". Aborting." << endl;
    return -1; 
  }

  // Read from serial
  char c;
  for(;;){
    serial >> c;
    cout << "Got: " << c << endl;
  }

  // Exit (will never get here tho)
  return 0;
}
