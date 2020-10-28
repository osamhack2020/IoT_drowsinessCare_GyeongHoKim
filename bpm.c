#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main()
{
    int pulseDV, cameraDV;
    int BPM, pulseFlag;
    int threshold = 65; // 임의의 값
    
    pulseDV = serialOpen("/dev/ttyACM0", 9600); // 아두이노에서 시리얼 통신

    while(1) {
        BPM = serialGetchar(pulseDV);
        if(BPM < threshold) pulseFlag = 1;
    }
}