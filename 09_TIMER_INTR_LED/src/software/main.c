#include <stdio.h>
#include "xparameters.h"
#include <sleep.h>
#include "xgpiops.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"

#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID
#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID

#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR //interrupt number

// PS LED pin location
#define MIO10_LED 10

#define TIMER_LOAD_VALUE 0x491D72A //200ms

XGpioPs Gpio;				/* The driver instance for GPIO Device. */
XScuTimer TimerInstance;	/* Cortex A9 Scu Private Timer Instance */
XScuGic IntcInstance;		/* Interrupt Controller Instance */

void mio_init() {
	XGpioPs_Config *ConfigPtr;
	// looks for the device configuration based on the unique device ID
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);
	// set GPIO direction to output, 0 as input, 1 as output
	// set output enable, 0 as disabled, 1 as enabled
	XGpioPs_SetDirectionPin(&Gpio, MIO10_LED, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, MIO10_LED, 1);
	// Set the GPIO output to be low. 0 light  1 -- not light
//	XGpioPs_WritePin(&Gpio, MIO10_LED, 0x1);
}

void timer_init() {
	XScuTimer_Config *ConfigPtr;

	//Initialize the Scu Private Timer driver.
	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	XScuTimer_CfgInitialize(&TimerInstance, ConfigPtr,ConfigPtr->BaseAddr);

	// enable auto reload mode
	XScuTimer_EnableAutoReload(&TimerInstance);
	// load initialization counter data
	XScuTimer_LoadTimer(&TimerInstance, TIMER_LOAD_VALUE);

}

void TimerIntrHandler(void *CallBackRef)
{
	XScuTimer *TimerInstancePtr = (XScuTimer *) CallBackRef;
	int led_status;
	if (XScuTimer_IsExpired(TimerInstancePtr)) {
		XScuTimer_ClearInterruptStatus(TimerInstancePtr);
		led_status = XGpioPs_ReadPin(&Gpio, MIO10_LED);
		//printf("led_status is : %d\n\r",led_status);
		led_status = ~led_status;
		XGpioPs_WritePin(&Gpio, MIO10_LED, led_status);
	}
}

void timer_intr_init() {
	XScuGic_Config *IntcConfig;

	// Initialize the interrupt controller driver
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	XScuGic_CfgInitialize(&IntcInstance, IntcConfig,IntcConfig->CpuBaseAddress);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
					(Xil_ExceptionHandler)XScuGic_InterruptHandler,
					&IntcInstance);
	Xil_ExceptionEnable();

	XScuGic_Connect(&IntcInstance, TIMER_IRPT_INTR,
					(Xil_ExceptionHandler)TimerIntrHandler,
					(void *)&TimerInstance);
	//enable intc interrupt
	XScuGic_Enable(&IntcInstance, TIMER_IRPT_INTR);

	//enable timer interrupt
	XScuTimer_EnableInterrupt(&TimerInstance);


}

int main() {
	mio_init();
	timer_init();
	timer_intr_init();
	printf("Timer Interrupt TEST!\n\r");
	XScuTimer_Start(&TimerInstance);
	while(1) {
	}

	return 0;
}


