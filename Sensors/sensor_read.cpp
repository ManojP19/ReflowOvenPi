#include "MAX31856.h"

// Test Program //
#define ThermocoupleBreakoutChipSelectGPIO 5

int main(void)
{
    struct MAX31856_S ThermocoupleBreakout;
    double ValR, coldJ;
    int k;

    if (gpioInitialise() < 0) {
	return 1;
    }
    ThermocoupleBreakout = SetupMAX31856(ThermocoupleBreakoutChipSelectGPIO, MAX31856_ThermocoupleType_K, MAX31856_Unit_Celcius, MAX31856_ColdJunction_ON);
    // Change any other setting here //
    ThermocoupleBreakout.AveragingMode = MAX31856_AveragingMode_4;
    MAX31856_CombineSettingsAndSend(&ThermocoupleBreakout);


    for(k=0;k<20;++k)
    {
        usleep(500000);

        ValR = MAX31856GetTemperature(&ThermocoupleBreakout);
        coldJ = MAX31856GetColdJunctionTemp(&ThermocoupleBreakout);
        printf("Cold Junct: %.5lf C;    Hot Junct: %.5lfC\n", coldJ, ValR);
    }

    MAX31856_Stop(ThermocoupleBreakout);
    gpioTerminate();
    return 0;
}
