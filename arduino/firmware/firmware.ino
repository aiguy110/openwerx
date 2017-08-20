/*************************************************** 
  This is an example for the Si4713 FM Radio Transmitter with RDS

  Designed specifically to work with the Si4713 breakout in the
  adafruit shop
  ----> https://www.adafruit.com/products/1958

  These transmitters use I2C to communicate, plus reset pin. 
  3 pins are required to interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

  Many thx to https://github.com/phrm/fmtx/blob/master/firmware/firmware.ino !

 ****************************************************/

#include <Wire.h>
#include <Adafruit_Si4713.h>

#define RESETPIN 12

#define FMSTATION 10230      // 10230 == 102.30 MHz

Adafruit_Si4713 radio = Adafruit_Si4713(RESETPIN);

int rstBtn1 = 3;
int rstBtn2 = 4;
int ledPin  = 5;

bool rst1_pressed(){
  return !digitalRead(rstBtn1);
}

bool rst2_pressed(){
  return !digitalRead(rstBtn2);
}

bool leds_on(){
  return !digitalRead(ledPin);
}

void setup() {
  Serial.begin(19200);
  
  pinMode(ledPin, INPUT);
  pinMode(rstBtn1, INPUT);
  pinMode(rstBtn2, INPUT);

  if (! radio.begin()) {  // begin with address 0x63 (CS high default)
    Serial.println("Couldn't find radio?");
    while (1);
  }

  // Uncomment to scan power of entire range from 87.5 to 108.0 MHz
  /*
  for (uint16_t f  = 8750; f<10800; f+=10) {
   radio.readTuneMeasure(f);
   Serial.print("Measuring "); Serial.print(f); Serial.print("...");
   radio.readTuneStatus();
   Serial.println(radio.currNoiseLevel);
   }
   */

  radio.setTXpower(115);  // dBuV, 88-115 max

  radio.tuneFM(FMSTATION); // 102.3 mhz

  // This will tell you the status in case you want to read it from the chip
  radio.readTuneStatus();
  
  // begin the RDS/RDBS transmission
  radio.beginRDS();
  radio.setRDSstation("AdaRadio");
  radio.setRDSbuffer( "Adafruit g0th Radio!");

  radio.setGPIOctrl(_BV(1) | _BV(2));  // set GP1 and GP2 to output
}



void loop() {
  radio.readASQ();
//  // toggle GPO1 and GPO2
//  radio.setGPIO(_BV(1));
//  delay(500);
//  radio.setGPIO(_BV(2));
//  delay(500);
  Serial.write(leds_on());
  Serial.write(rst1_pressed());
  Serial.write(rst2_pressed());
  Serial.write('\n');
  delay(10);
}
