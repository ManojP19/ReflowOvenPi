#include "thermocouple.h"
#include "MAX31856.h"

#define ThermocoupleBreakoutChipSelectGPIO 5
#define NumberOfSensors 3
#define SensorReadFrequency 5    // The frequency of one sensor being read (Hz)

int main(void) {
    struct MAX31856_S ThermocoupleBreakout;
    double temperature[NumberOfSensors];
    int sensor_select = NumberOfSensors - 1;
    double* temperature_pointer;
    int sleep_time;

    if (gpioInitialise() < 0) {
        return 1;
    }

    // Set MUX pins
    gpioSetMode(22, PI_OUTPUT);     // MUX bit0
    gpioSetMode(23, PI_OUTPUT);     // MUX bit1
    gpioSetMode(24, PI_OUTPUT);     // MUX bit2
    gpioSetMode(25, PI_OUTPUT);     // MUX bit3
    gpioSetMode(27, PI_OUTPUT);     // MUX enable

    gpioWrite(27, 0);    	    // enable MUX

    // Setup MAX31856 Breakout
    ThermocoupleBreakout = SetupMAX31856(ThermocoupleBreakoutChipSelectGPIO, MAX31856_ThermocoupleType_K, MAX31856_Unit_Celcius, MAX31856_ColdJunction_ON);
    ThermocoupleBreakout.AveragingMode = MAX31856_AveragingMode_4;
    MAX31856_CombineSettingsAndSend(&ThermocoupleBreakout);

    // Initialise pointer to temperature array containig data for each sensor **Can add a data log with all prev temps**
    temperature_pointer = temperature;

    // Sleep time between sensor reads
    sleep_time = 1000000 / (NumberOfSensors * SensorReadFrequency);

    // Initialise temperature array
    for (int i = 0; i < NumberOfSensors; i++) {
        *temperature_pointer = 0;
        temperature_pointer++;
    }
    temperature_pointer = temperature;     // Reset pointer

    for (int i = 0; i < 30; i++) {
        sensor_select = (sensor_select + 1) % NumberOfSensors;
        select_MUX(sensor_select);
        usleep(sleep_time / 2);
        *temperature_pointer = MAX31856GetTemperature(&ThermocoupleBreakout);
        printf("S%d: %.5lf C;  ", sensor_select+1, *temperature_pointer);
	temperature_pointer++;
        usleep(sleep_time / 2);
        if (sensor_select == NumberOfSensors - 1) {
	    temperature_pointer = temperature;
	    printf("\n");
	}
    }
    gpioWrite(27, 1);     	    // disable MUX
    MAX31856_Stop(ThermocoupleBreakout);
    gpioTerminate();		    // terminate Pi GPIO
    return 0;
}
