#include <stdio.h>
#include <wiringPi.h>
#define PIN_NUM 27 // gpio 16

int main()
{
    if(wiringPiSetup() == -1) return -1;
    pinMode(PIN_NUM, OUTPUT);

    while(1) {
        digitalWrite(PIN_NUM, 1);
        delay(1000);
        digitalWrite(PIN_NUM, 0);
        delay(250);
    }
    return 0;
}