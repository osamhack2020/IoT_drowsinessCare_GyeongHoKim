#include <pigpiod_if2.h>
#include <thread>
#include <unistd.h>
 
using namespace std;
 
#define PIN 13
 
void play_beep()
{
   auto instance = pigpio_start(NULL, NULL);
   set_PWM_frequency(instance, PIN, 2700);
 
   // 128/255 = 50% duty
   set_PWM_dutycycle(instance, PIN, 128);
 
   // play beep for 100 milliseconds
   this_thread::sleep_for(chrono::milliseconds(100));
 
   // turn off beep
   set_PWM_dutycycle(instance, PIN, 0);
 
   pigpio_stop(instance);
}

int main()
{
    while(1) {
        play_beep();
        sleep(2);
    }
}