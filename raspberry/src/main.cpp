#include <wiringPi.h>
#include <wiringPiI2C.h>

#define PIN_NUM 0 // WirePi_0 -> GPIO_17

int main (void)
{
    wiringPiSetup ();
    pinMode (PIN_NUM, OUTPUT);
    for (;){
        digitalWrite (PIN_NUM, HIGH) ; delay (500) ;
        digitalWrite (PIN_NUM,  LOW) ; delay (500) ;
    }
    return PIN_NUM ;
}
