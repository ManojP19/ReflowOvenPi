#include <stdio.h>
#include <math.h>
#include <pigpio.h>
#include <unistd.h>

void bin_convert(int* array, int number, int bits=3) {
    int temp,bin_digit;
    if (bits==3) {
         array++;
    }
    for (int i = 4 - bits; i < 4; i++) {
        bin_digit = pow(2,3-i);
	temp = number / bin_digit;
        number -= temp*bin_digit;
	*array = temp;
        array++;
    }
}

void select_MUX(int sensor) {
    int bin_array[4] = {0,0,0,0};
    int* bin_pointer = bin_array;
    // gpio_set pulls high, gpio_clr pulls low
    bin_convert(bin_array, sensor);
    for (int pin = 25; pin > 21; pin--) {
        gpioWrite(pin, *bin_pointer);
    //    printf("pin: %d: %d     ", pin, *bin_pointer);
        bin_pointer++;
    }
    //printf("\n");
}

int main() {
    if (gpioInitialise() < 0) {    // initialise Pi GPIO, check error
	return 1;
    }
    // Set MUX pins
    gpioSetMode(22, PI_OUTPUT);    // MUX bit0
    gpioSetMode(23, PI_OUTPUT);    // MUX bit1
    gpioSetMode(24, PI_OUTPUT);    // MUX bit2
    gpioSetMode(25, PI_OUTPUT);    // MUX bit3
    gpioSetMode(27, PI_OUTPUT);    // MUX enable
    
    gpioWrite(27, 0);              // enable MUX
    int bits[4] = {0,0,0,0};
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 3; j++) {
 	    bin_convert(bits, j, 4);
            printf("\rLights On");
            fflush(stdout); 
           // printf("V1: dec %d: bin %d %d %d %d\n",j,bits[0],bits[1],bits[2],bits[3]);
            select_MUX(j);
           // usleep(1000*31.25);                   // program sleeps 1 second
            usleep(1000*125);
        }
    }
    printf("\rLights off\n");
    gpioWrite(27, 1);              // disable MUX
    gpioTerminate();   		   // terminate Pi GPIO
    return 0;
}
