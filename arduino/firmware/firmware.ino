/***************************************************
 * This is an example for the Si4713 FM Radio Transmitter with RDS
 *
 * Designed specifically to work with the Si4713 breakout in the
 * adafruit shop
 * ----> https://www.adafruit.com/products/1958
 *
 * These transmitters use I2C to communicate, plus reset pin.
 * 3 pins are required to interface
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 * BSD license, all text above must be included in any redistribution
 *
 * Many thx to https://github.com/phrm/fmtx/blob/master/firmware/firmware.ino !
 *
 ****************************************************/

#include <Wire.h>
#include <Adafruit_Si4713.h>
#include <Servo.h>

Servo servo_0;  // create servo object to control a servo
Servo servo_1;  // create servo object to control a servo

#define RESETPIN 12

int FMSTATION= 10230;      // 10230 == 102.30 MHz

//display macros
#define    D0_PIN   10 // OLED PIN: 7
#define    D1_PIN   9 // OLED PIN: 8
#define    DC_PIN   11 // OLED PIN: 4
#define    RES_PIN  8 // OLED PIN: 16
#define    CS_PIN   7 // OLED PIN: 17
#define    RED      0x0000FF
#define    GREEN    0x00FF00
#define    BLUE     0xFF0000
#define    WHITE    0xFFFFFF
#define    BLACK    0x000000


Adafruit_Si4713 radio = Adafruit_Si4713(RESETPIN);

char buff[] = " "; //8 bytes

//messages that could be recieved
const String incomingMessages[3] = {
  "speaker",
  "transmitter",
  "paper"
};

unsigned char Ascii_1[97][5] = {     // Refer to "Times New Roman" Font Database...
  //   Basic Characters
  {
    0x00,0x00,0x00,0x00,0x00      }
  ,     //   (  0)    - 0x0000 Empty set
  {
    0x00,0x00,0x4F,0x00,0x00      }
  ,     //   (  1)  ! - 0x0021 Exclamation Mark
  {
    0x00,0x07,0x00,0x07,0x00      }
  ,     //   (  2)  " - 0x0022 Quotation Mark
  {
    0x14,0x7F,0x14,0x7F,0x14      }
  ,     //   (  3)  # - 0x0023 Number Sign
  {
    0x24,0x2A,0x7F,0x2A,0x12      }
  ,     //   (  4)  $ - 0x0024 Dollar Sign
  {
    0x23,0x13,0x08,0x64,0x62      }
  ,     //   (  5)  % - 0x0025 Percent Sign
  {
    0x36,0x49,0x55,0x22,0x50      }
  ,     //   (  6)  & - 0x0026 Ampersand
  {
    0x00,0x05,0x03,0x00,0x00      }
  ,     //   (  7)  ' - 0x0027 Apostrophe
  {
    0x00,0x1C,0x22,0x41,0x00      }
  ,     //   (  8)  ( - 0x0028 Left Parenthesis
  {
    0x00,0x41,0x22,0x1C,0x00      }
  ,     //   (  9)  ) - 0x0029 Right Parenthesis
  {
    0x14,0x08,0x3E,0x08,0x14      }
  ,     //   ( 10)  * - 0x002A Asterisk
  {
    0x08,0x08,0x3E,0x08,0x08      }
  ,     //   ( 11)  + - 0x002B Plus Sign
  {
    0x00,0x50,0x30,0x00,0x00      }
  ,     //   ( 12)  , - 0x002C Comma
  {
    0x08,0x08,0x08,0x08,0x08      }
  ,     //   ( 13)  - - 0x002D Hyphen-Minus
  {
    0x00,0x60,0x60,0x00,0x00      }
  ,     //   ( 14)  . - 0x002E Full Stop
  {
    0x20,0x10,0x08,0x04,0x02      }
  ,     //   ( 15)  / - 0x002F Solidus
  {
    0x3E,0x51,0x49,0x45,0x3E      }
  ,     //   ( 16)  0 - 0x0030 Digit Zero
  {
    0x00,0x42,0x7F,0x40,0x00      }
  ,     //   ( 17)  1 - 0x0031 Digit One
  {
    0x42,0x61,0x51,0x49,0x46      }
  ,     //   ( 18)  2 - 0x0032 Digit Two
  {
    0x21,0x41,0x45,0x4B,0x31      }
  ,     //   ( 19)  3 - 0x0033 Digit Three
  {
    0x18,0x14,0x12,0x7F,0x10      }
  ,     //   ( 20)  4 - 0x0034 Digit Four
  {
    0x27,0x45,0x45,0x45,0x39      }
  ,     //   ( 21)  5 - 0x0035 Digit Five
  {
    0x3C,0x4A,0x49,0x49,0x30      }
  ,     //   ( 22)  6 - 0x0036 Digit Six
  {
    0x01,0x71,0x09,0x05,0x03      }
  ,     //   ( 23)  7 - 0x0037 Digit Seven
  {
    0x36,0x49,0x49,0x49,0x36      }
  ,     //   ( 24)  8 - 0x0038 Digit Eight
  {
    0x06,0x49,0x49,0x29,0x1E      }
  ,     //   ( 25)  9 - 0x0039 Dight Nine
  {
    0x00,0x36,0x36,0x00,0x00      }
  ,     //   ( 26)  : - 0x003A Colon
  {
    0x00,0x56,0x36,0x00,0x00      }
  ,     //   ( 27)  ; - 0x003B Semicolon
  {
    0x08,0x14,0x22,0x41,0x00      }
  ,     //   ( 28)  < - 0x003C Less-Than Sign
  {
    0x14,0x14,0x14,0x14,0x14      }
  ,     //   ( 29)  = - 0x003D Equals Sign
  {
    0x00,0x41,0x22,0x14,0x08      }
  ,     //   ( 30)  > - 0x003E Greater-Than Sign
  {
    0x02,0x01,0x51,0x09,0x06      }
  ,     //   ( 31)  ? - 0x003F Question Mark
  {
    0x32,0x49,0x79,0x41,0x3E      }
  ,     //   ( 32)  @ - 0x0040 Commercial At
  {
    0x7E,0x11,0x11,0x11,0x7E      }
  ,     //   ( 33)  A - 0x0041 Latin Capital Letter A
  {
    0x7F,0x49,0x49,0x49,0x36      }
  ,     //   ( 34)  B - 0x0042 Latin Capital Letter B
  {
    0x3E,0x41,0x41,0x41,0x22      }
  ,     //   ( 35)  C - 0x0043 Latin Capital Letter C
  {
    0x7F,0x41,0x41,0x22,0x1C      }
  ,     //   ( 36)  D - 0x0044 Latin Capital Letter D
  {
    0x7F,0x49,0x49,0x49,0x41      }
  ,     //   ( 37)  E - 0x0045 Latin Capital Letter E
  {
    0x7F,0x09,0x09,0x09,0x01      }
  ,     //   ( 38)  F - 0x0046 Latin Capital Letter F
  {
    0x3E,0x41,0x49,0x49,0x7A      }
  ,     //   ( 39)  G - 0x0047 Latin Capital Letter G
  {
    0x7F,0x08,0x08,0x08,0x7F      }
  ,     //   ( 40)  H - 0x0048 Latin Capital Letter H
  {
    0x00,0x41,0x7F,0x41,0x00      }
  ,     //   ( 41)  I - 0x0049 Latin Capital Letter I
  {
    0x20,0x40,0x41,0x3F,0x01      }
  ,     //   ( 42)  J - 0x004A Latin Capital Letter J
  {
    0x7F,0x08,0x14,0x22,0x41      }
  ,     //   ( 43)  K - 0x004B Latin Capital Letter K
  {
    0x7F,0x40,0x40,0x40,0x40      }
  ,     //   ( 44)  L - 0x004C Latin Capital Letter L
  {
    0x7F,0x02,0x0C,0x02,0x7F      }
  ,     //   ( 45)  M - 0x004D Latin Capital Letter M
  {
    0x7F,0x04,0x08,0x10,0x7F      }
  ,     //   ( 46)  N - 0x004E Latin Capital Letter N
  {
    0x3E,0x41,0x41,0x41,0x3E      }
  ,     //   ( 47)  O - 0x004F Latin Capital Letter O
  {
    0x7F,0x09,0x09,0x09,0x06      }
  ,     //   ( 48)  P - 0x0050 Latin Capital Letter P
  {
    0x3E,0x41,0x51,0x21,0x5E      }
  ,     //   ( 49)  Q - 0x0051 Latin Capital Letter Q
  {
    0x7F,0x09,0x19,0x29,0x46      }
  ,     //   ( 50)  R - 0x0052 Latin Capital Letter R
  {
    0x46,0x49,0x49,0x49,0x31      }
  ,     //   ( 51)  S - 0x0053 Latin Capital Letter S
  {
    0x01,0x01,0x7F,0x01,0x01      }
  ,     //   ( 52)  T - 0x0054 Latin Capital Letter T
  {
    0x3F,0x40,0x40,0x40,0x3F      }
  ,     //   ( 53)  U - 0x0055 Latin Capital Letter U
  {
    0x1F,0x20,0x40,0x20,0x1F      }
  ,     //   ( 54)  V - 0x0056 Latin Capital Letter V
  {
    0x3F,0x40,0x38,0x40,0x3F      }
  ,     //   ( 55)  W - 0x0057 Latin Capital Letter W
  {
    0x63,0x14,0x08,0x14,0x63      }
  ,     //   ( 56)  X - 0x0058 Latin Capital Letter X
  {
    0x07,0x08,0x70,0x08,0x07      }
  ,     //   ( 57)  Y - 0x0059 Latin Capital Letter Y
  {
    0x61,0x51,0x49,0x45,0x43      }
  ,     //   ( 58)  Z - 0x005A Latin Capital Letter Z
  {
    0x00,0x7F,0x41,0x41,0x00      }
  ,     //   ( 59)  [ - 0x005B Left Square Bracket
  {
    0x02,0x04,0x08,0x10,0x20      }
  ,     //   ( 60)  \ - 0x005C Reverse Solidus
  {
    0x00,0x41,0x41,0x7F,0x00      }
  ,     //   ( 61)  ] - 0x005D Right Square Bracket
  {
    0x04,0x02,0x01,0x02,0x04      }
  ,     //   ( 62)  ^ - 0x005E Circumflex Accent
  {
    0x40,0x40,0x40,0x40,0x40      }
  ,     //   ( 63)  _ - 0x005F Low Line
  {
    0x01,0x02,0x04,0x00,0x00      }
  ,     //   ( 64)  ` - 0x0060 Grave Accent
  {
    0x20,0x54,0x54,0x54,0x78      }
  ,     //   ( 65)  a - 0x0061 Latin Small Letter A
  {
    0x7F,0x48,0x44,0x44,0x38      }
  ,     //   ( 66)  b - 0x0062 Latin Small Letter B
  {
    0x38,0x44,0x44,0x44,0x20      }
  ,     //   ( 67)  c - 0x0063 Latin Small Letter C
  {
    0x38,0x44,0x44,0x48,0x7F      }
  ,     //   ( 68)  d - 0x0064 Latin Small Letter D
  {
    0x38,0x54,0x54,0x54,0x18      }
  ,     //   ( 69)  e - 0x0065 Latin Small Letter E
  {
    0x08,0x7E,0x09,0x01,0x02      }
  ,     //   ( 70)  f - 0x0066 Latin Small Letter F
  {
    0x06,0x49,0x49,0x49,0x3F      }
  ,     //   ( 71)  g - 0x0067 Latin Small Letter G
  {
    0x7F,0x08,0x04,0x04,0x78      }
  ,     //   ( 72)  h - 0x0068 Latin Small Letter H
  {
    0x00,0x44,0x7D,0x40,0x00      }
  ,     //   ( 73)  i - 0x0069 Latin Small Letter I
  {
    0x20,0x40,0x44,0x3D,0x00      }
  ,     //   ( 74)  j - 0x006A Latin Small Letter J
  {
    0x7F,0x10,0x28,0x44,0x00      }
  ,     //   ( 75)  k - 0x006B Latin Small Letter K
  {
    0x00,0x41,0x7F,0x40,0x00      }
  ,     //   ( 76)  l - 0x006C Latin Small Letter L
  {
    0x7C,0x04,0x18,0x04,0x7C      }
  ,     //   ( 77)  m - 0x006D Latin Small Letter M
  {
    0x7C,0x08,0x04,0x04,0x78      }
  ,     //   ( 78)  n - 0x006E Latin Small Letter N
  {
    0x38,0x44,0x44,0x44,0x38      }
  ,     //   ( 79)  o - 0x006F Latin Small Letter O
  {
    0x7C,0x14,0x14,0x14,0x08      }
  ,     //   ( 80)  p - 0x0070 Latin Small Letter P
  {
    0x08,0x14,0x14,0x18,0x7C      }
  ,     //   ( 81)  q - 0x0071 Latin Small Letter Q
  {
    0x7C,0x08,0x04,0x04,0x08      }
  ,     //   ( 82)  r - 0x0072 Latin Small Letter R
  {
    0x48,0x54,0x54,0x54,0x20      }
  ,     //   ( 83)  s - 0x0073 Latin Small Letter S
  {
    0x04,0x3F,0x44,0x40,0x20      }
  ,     //   ( 84)  t - 0x0074 Latin Small Letter T
  {
    0x3C,0x40,0x40,0x20,0x7C      }
  ,     //   ( 85)  u - 0x0075 Latin Small Letter U
  {
    0x1C,0x20,0x40,0x20,0x1C      }
  ,     //   ( 86)  v - 0x0076 Latin Small Letter V
  {
    0x3C,0x40,0x30,0x40,0x3C      }
  ,     //   ( 87)  w - 0x0077 Latin Small Letter W
  {
    0x44,0x28,0x10,0x28,0x44      }
  ,     //   ( 88)  x - 0x0078 Latin Small Letter X
  {
    0x0C,0x50,0x50,0x50,0x3C      }
  ,     //   ( 89)  y - 0x0079 Latin Small Letter Y
  {
    0x44,0x64,0x54,0x4C,0x44      }
  ,     //   ( 90)  z - 0x007A Latin Small Letter Z
  {
    0x00,0x08,0x36,0x41,0x00      }
  ,     //   ( 91)  { - 0x007B Left Curly Bracket
  {
    0x00,0x00,0x7F,0x00,0x00      }
  ,     //   ( 92)  | - 0x007C Vertical Line
  {
    0x00,0x41,0x36,0x08,0x00      }
  ,     //   ( 93)  } - 0x007D Right Curly Bracket
  {
    0x02,0x01,0x02,0x04,0x02      }
  ,     //   ( 94)  ~ - 0x007E Tilde
  {
    0x08,0x0C,0x0E,0x0C,0x08      }
  ,     //   ( 95)  upward facing triangle/arrow
  {
    0x08,0x18,0x38,0x18,0x08      }
  ,     //   ( 96)  downward facing triangle/arrow
};

void OLED_Command(unsigned char c)        // send command to OLED
{
  unsigned char i;
  unsigned char mask = 0x80;
  digitalWrite(CS_PIN, LOW);
  digitalWrite(DC_PIN, LOW);
  for(i=0;i<8;i++)
  {
    digitalWrite(D0_PIN, LOW);
    if((c & mask) >> 7 == 1)
    {
      digitalWrite(D1_PIN, HIGH);
    }
    else

    {
      digitalWrite(D1_PIN, LOW);
    }
    digitalWrite(D0_PIN, HIGH);
    c = c << 1;
  }
  digitalWrite(CS_PIN, HIGH);
}

void OLED_Data(unsigned char d)        // send data to OLED
{
  unsigned char i;
  unsigned char mask = 0x80;

  digitalWrite(CS_PIN, LOW);
  digitalWrite(DC_PIN, HIGH);
  for(i=0;i<8;i++)
  {
    digitalWrite(D0_PIN, LOW);
    if((d & mask) >> 7 == 1)
    {
      digitalWrite(D1_PIN, HIGH);
    }
    else
    {
      digitalWrite(D1_PIN, LOW);
    }
    digitalWrite(D0_PIN, HIGH);
    d = d << 1;
  }
  digitalWrite(CS_PIN, HIGH);

}

void OLED_SetColumnAddress(unsigned char x_start, unsigned char x_end)    // set column address start + end
{
  OLED_Command(0x15);
  OLED_Data(x_start);
  OLED_Data(x_end);
}

void OLED_SetRowAddress(unsigned char y_start, unsigned char y_end)    // set row address start + end
{
  OLED_Command(0x75);
  OLED_Data(y_start);
  OLED_Data(y_end);
}

void OLED_WriteMemoryStart(void)    // write to RAM command
{
  OLED_Command(0x5C);
}

void OLED_Pixel(unsigned long color)    // write one pixel of a given color
{
  OLED_Data((color>>16));
  OLED_Data((color>>8));
  OLED_Data(color);
}

void OLED_FillScreen(unsigned long color)    // fill screen with a given color
{
  unsigned int i,j;
  OLED_SetColumnAddress(0x00, 0x7F);
  OLED_SetRowAddress(0x00, 0x7F);
  OLED_WriteMemoryStart();
  for(i=0;i<128;i++)
  {
    for(j=0;j<128;j++)
    {
      OLED_Pixel(color);
    }
  }
}


// DRAW TEXT

void OLED_Text(unsigned char x_pos, unsigned char y_pos, unsigned char letter, unsigned long textColor, unsigned long backgroundColor)  // function to show text
{
  int i;
  int count;
  unsigned char mask = 0x80;

  for(i=0;i<8;i++)     //each character is 8 px tall
  {
    OLED_SetColumnAddress(x_pos, 0x7F);
    OLED_SetRowAddress(y_pos, 0x7F);
    OLED_WriteMemoryStart();
    for (count=0;count<5;count++)    //each character is 5px wide
    {
      if((Ascii_1[letter][count] & mask) == mask)
        OLED_Pixel(textColor);
      else
        OLED_Pixel(backgroundColor);
    }
    y_pos++;
    mask = mask >> 1;
  }
}

// DRAW TEXT, 2X SIZE
void OLED_Text2X(unsigned char x_pos, unsigned char y_pos, unsigned char letter, unsigned long textColor, unsigned long backgroundColor)  // function to show text (2x size)
{
  int i;
  int count;
  unsigned char mask = 0x80;

  for(i=1;i<=16;i++)     // each character is 16 pixels tall
  {
    OLED_SetColumnAddress(x_pos, 0x7F);
    OLED_SetRowAddress(y_pos, 0x7F);
    OLED_WriteMemoryStart();
    for (count=0;count<10;count++)    // each character is 10 pixels wide
    {
      if((Ascii_1[letter][(count/2)] & mask) == mask)
        OLED_Pixel(textColor);
      else
        OLED_Pixel(backgroundColor);
    }
    y_pos++;
    if((i%2)==0)
    {
      mask = mask >> 1;
    }
  }
}


// INITIALIZE OLED
void OLED_Init(void)      //OLED initialization
{
  digitalWrite(RES_PIN, LOW);
  delay(500);
  digitalWrite(RES_PIN, HIGH);
  delay(500);

  OLED_Command(0xFD);	// Command lock setting
  OLED_Data(0x12);		// unlock
  OLED_Command(0xFD);	// Command lock setting
  OLED_Data(0xB1);		// unlock
  OLED_Command(0xAE);
  OLED_Command(0xB3);	// clock & frequency
  OLED_Data(0xF1);		// clock=Diviser+1 frequency=fh
  OLED_Command(0xCA);	// Duty
  OLED_Data(0x7F);		// OLED _END+1
  OLED_Command(0xA2);  	// Display offset
  OLED_Data(0x00);
  OLED_Command(0xA1);	// Set display start line
  OLED_Data(0x00);		// 0x00 start line
  OLED_Command(0xA0);	// Set Re-map, color depth
  OLED_Data(0xA0);		// 8-bit 262K
  OLED_Command(0xB5);	// set GPIO
  OLED_Data(0x00);		// disabledFS
  OLED_Command(0xAB);	// Function Set
  OLED_Data(0x01);		// 8-bit interface, internal VDD regulator
  OLED_Command(0xB4);	// set VSL
  OLED_Data(0xA0);		// external VSL
  OLED_Data(0xB5);
  OLED_Data(0x55);
  OLED_Command(0xC1);	// Set contrast current for A,B,C
  OLED_Data(0x8A);		// Color A
  OLED_Data(0x70);		// Color B
  OLED_Data(0x8A);		// Color C
  OLED_Command(0xC7);	// Set master contrast
  OLED_Data(0x0F);		//
  OLED_Command(0xB9);	// use linear grayscale LUT
  OLED_Command(0xB1);	// Set pre & dis-charge
  OLED_Data(0x32);		// pre=1h, dis=1h
  OLED_Command(0xBB);	// Set precharge voltage of color A,B,C
  OLED_Data(0x07);		//
  OLED_Command(0xB2);       // display enhancement
  OLED_Data(0xa4);
  OLED_Data(0x00);
  OLED_Data(0x00);
  OLED_Command(0xB6);	// precharge period
  OLED_Data(0x01);
  OLED_Command(0xBE);	// Set VcomH
  OLED_Data(0x07);
  OLED_Command(0xA6);	// Normal display
  OLED_Command(0xAF);	// Display on
}

void draw_String(String text){
  int textLength = text.length();
  int row = 120;
  int col = 0;
  for(int i = 0; i < textLength; i++){
    if(col > 16){
      row -= 10;
      col = 0;
    }
    OLED_Text( 5 + (col * 7), row, (int)text[i]-32, WHITE, BLACK);
    col++;
  }
}

void draw_String(String text, int row){
  int textLength = text.length();
  int col = 0;
  for(int i = 0; i < textLength; i++){
    if(col > 16){
      row -= 10;
      col = 0;
    }
    OLED_Text( 5 + (col * 7), row, (int)text[i]-32, WHITE, BLACK);
    col++;
  }
}


void draw_String2X(String text, int row){
  int textLength = text.length();
  int col = 0;
  for(int i = 0; i < textLength; i++){
    if(col > 16){
      row -= 10;
      col = 0;
    }
    OLED_Text2X( 5 + (col * 7), row, (int)text[i]-32, WHITE, BLACK);
    col++;
  }
}


void initRadio(int freq){
  if (! radio.begin()) {  // begin with address 0x63 (CS high default)
    Serial.println("Couldn't find radio?");
    while (1);
  }

  radio.setTXpower(115);  // dBuV, 88-115 max

  radio.tuneFM(freq); // 102.3 mhz

  // This will tell you the status in case you want to read it from the chip
  radio.readTuneStatus();

  // begin the RDS/RDBS transmission
  radio.beginRDS();
  radio.setRDSstation("AdaRadio");
  radio.setRDSbuffer( "Adafruit g0th Radio!");

  radio.setGPIOctrl(_BV(1) | _BV(2));  // set GP1 and GP2 to output
}


void blocking_read(char buffer[]){
  int i = 0;
  boolean reading = false;
  while(1){
    if(Serial.available()){ // if data has been received
      char inByte = Serial.read(); // read the incoming data
      if(inByte=='{'){
        reading = true;
      }
      else if(inByte== '}' && reading){
        buffer[i]=0;
        return;
      }
      else{
        buffer[i]=inByte;
        i++;
      }
    }
  }
}


void setup() {
  Serial.begin(19200);

  // Setup servos
  servo_0.attach(5);  // attaches the servo on pin 9 to the servo object
  servo_1.attach(6);  // attaches the servo on pin 9 to the servo object

  // SETUP OLED Display
  pinMode(DC_PIN, OUTPUT);                        // configure DC_PIN as output
  pinMode(RES_PIN, OUTPUT);                       // configure RES_PIN as output
  pinMode(CS_PIN, OUTPUT);                        // configure CS_PIN as output
  digitalWrite(CS_PIN, HIGH);                     // set CS_PIN

  pinMode(D1_PIN, OUTPUT);                   // configure D1_PIN as output
  pinMode(D0_PIN, OUTPUT);                   // configure D0_PIN as output
  digitalWrite(D1_PIN, LOW);
  digitalWrite(D0_PIN, LOW);

  OLED_Init();                           // initialize display
}


void loop() {
  //  Serial.println("Entering echo loop...");
  //  char buffer[100];
  //  while(1){
  //    blocking_read(buffer);
  //    Serial.write('{');
  //    Serial.print(buffer);
  //    Serial.write('}');
  //  }

  OLED_FillScreen(BLACK);                // fill screen with black

  // Do initial handshake
  char buff[100];
  while(1){
    blocking_read(buff);
    if (strcmp(buff, "bitches")==0){
      Serial.println("{hell yeah}");
      break;
    }else{
      Serial.println(buff);
      Serial.println("{bad handshake}");
    }
  }

  // Get the name of the drone module being used
  char module_name[20];
  blocking_read(module_name);

  // If the name is "transmitter", hold here and wait for frequency info
  if (strcmp(module_name, "transmitter")==0){
    char freq_str[10];
    blocking_read(freq_str);
    int freq = String(freq_str).toInt();
    initRadio(freq);
  }

  int val = 0;
  boolean lights_on;
  boolean do_speaker = strcmp(module_name, "speaker")==0;
  boolean do_servos = strcmp(module_name, "paper")==0;
  boolean do_radio = strcmp(module_name, "transmitter")==0;
  if(do_speaker) Serial.print("Doing speaker");
  if(do_servos) Serial.print("Doing servos");
  if(do_radio) Serial.print("Doing radio");
  while(1){
    // Are the drone LEDs on?
    val = analogRead(A1);
    if(val >= 700){
      lights_on = true;
      draw_String2X("ON ", 100);
      Serial.print("{1}");
    }
    else{
      lights_on = false;
      draw_String2X("OFF", 100);
      Serial.print("{0}");
    }

    // Maybe do servo stuff
    if (do_servos){
      if(lights_on){
        servo_0.write(0);
        servo_1.write(0);
      }
      else{
        servo_0.write(180);
        servo_1.write(180);
      }
    }


    if (do_radio) radio.readASQ();

    Serial.write('\n');
    //draw_String(Display_Output[2]);

  }

  // wait here forever

}
