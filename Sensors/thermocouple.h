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
        temp = number / bin_digit; number -= temp*bin_digit; 
        *array = temp; array++; 
        } 
    } 
    
void select_MUX(int sensor) { 
    int bin_array[4] = {0,0,0,0}; 
    int* bin_pointer = bin_array; 
    // gpio_set pulls high, gpio_clr pulls low 
    bin_convert(bin_array, sensor); 
    for (int pin = 25; pin > 21; pin--) { 
        gpioWrite(pin, *bin_pointer); 
        // printf("pin: %d: %d ", pin, *bin_pointer); 
        bin_pointer++; 
    } 
    //printf("\n"); 
}
