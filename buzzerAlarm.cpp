#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
const int pinPiezo = 13; //gpio 9번
const int aMelody[8] = {131,147,165,175,196,220,247,262};
int main(void)
{
    wiringPiSetupGpio();
    softToneCreate(pinPiezo);
    while(1) {
        int i;
        for(i=0;i<8;i++)
            if(aMelody[i]%2==0) {
                softToneWrite(pinPiezo,aMelody[i]); delay(1000);
            }
        softToneWrite(pinPiezo,0);
        delay(1000);
    }
    return 0;
}