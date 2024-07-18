
#include <stdio.h>
#include "xparameters.h"
#include <sleep.h>
#include "xgpiops.h"

#ifndef GPIO_DEVICE_ID
#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID
#endif

// PS LED pin location
#define MIO10_LED 10

XGpioPs Gpio;	/* The driver instance for GPIO Device. */

int main() {
	printf("GPIO MIO TEST!\n\r");

	XGpioPs_Config *ConfigPtr;

	// looks for the device configuration based on the unique device ID
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);

	// initialize GPIO driver
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);

	// set GPIO direction to output, 0 as input, 1 as output
	XGpioPs_SetDirectionPin(&Gpio, MIO10_LED, 1);

	// set output enable, 0 as disabled, 1 as enabled
	XGpioPs_SetOutputEnablePin(&Gpio, MIO10_LED, 1);

	// Set the GPIO output to be low. 0 light  1 -- not light
	XGpioPs_WritePin(&Gpio, MIO10_LED, 0x0);

	while(1) {
	// light led
		XGpioPs_WritePin(&Gpio, MIO10_LED, 0x0);
	// delay
		usleep(500000);
	// delight
		XGpioPs_WritePin(&Gpio, MIO10_LED, 0x1);
	// delay
		usleep(500000);
	}

	return 0;
}
