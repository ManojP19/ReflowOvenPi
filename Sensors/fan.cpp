
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>

#define frequency 100       // pwm frequency in Hz
#define us2seconds 1000000  // 1,000,000 micro seconds in a second

int main(void) {
//    int duty_cycle = 100;
    int sleep_time;
    if (gpioInitialise() < 0) {
        return 1;
    }
    gpioSetMode(26, PI_OUTPUT);
    sleep_time = us2seconds / frequency;
    for (int duty_cycle = 5; duty_cycle < 11; duty_cycle++) {
        for (int i = 0; i < 50; i++) {
            printf("\rFan in Operation");
            fflush(stdout);
            gpioWrite(26, 1);
            usleep(duty_cycle * 10 * sleep_time / 100);
            gpioWrite(26, 0);
            usleep((100 - duty_cycle * 10) * sleep_time / 100);
        }
    }
   // gpioWrite(26, 1);
   // usleep(1000000*10);
    gpioWrite(26, 0);
    printf("Fan off\n");
    gpioTerminate();
}
