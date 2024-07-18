#include <stdio.h>
#include "xparameters.h"
#include "xgpiops.h"
#include "xscugic.h"
#include "sleep.h"
#include "xgpio.h"

#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID		//PS GPIO ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID	//INTC ID
#define AXI_GPIO_DEVICE_ID	XPAR_GPIO_0_DEVICE_ID			//AXI GPIO ID

//AXI GPIO INTC number is 61
#define AXI_GPIO_INTERRUPT_ID	XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR

#define MIO10_LED  10
//AXI GPIO channel
#define AXI_GPIO_CHANNEL1		1

static XGpioPs Gpio; /* The Instance of the GPIO Driver */
static XScuGic Intc; /* The Instance of the Interrupt Controller Driver */
static XGpio   AXI_Gpio;

XGpioPs_Config *ConfigPtr;
XScuGic_Config *IntcConfig; /* Instance of the interrupt controller */

void SetupInterruptSystem(XScuGic *GicInstancePtr, XGpio *AXI_Gpio, u16 AXI_GpioIntrId);
//void IntrHandler(void *CallBackRef, u32 Bank, u32 Status);
void IntrHandler();
u32 key_press;

int main(void) {
	u32 led_value = 0;
	printf("AXI GPIO INTERRUPT TEST!\n\r");

	//initialize
	//configure and initialize PS GPIO
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);

	//initialize AXI GPIO
	XGpio_Initialize(&AXI_Gpio, AXI_GPIO_DEVICE_ID);

	// set PS LED to output and enable output
	XGpioPs_SetDirectionPin(&Gpio, MIO10_LED, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, MIO10_LED, 1);
	XGpioPs_WritePin(&Gpio, MIO10_LED, led_value);

	// set PL KEY to input
	XGpio_SetDataDirection(&AXI_Gpio, AXI_GPIO_CHANNEL1, 0x00000001);


	//void XGpio_InterruptClear(XGpio *InstancePtr, u32 Mask);
	SetupInterruptSystem(&Intc, &AXI_Gpio, AXI_GPIO_INTERRUPT_ID);

	while(1){
		if(key_press) {
			//
			if(XGpio_DiscreteRead(&AXI_Gpio, AXI_GPIO_CHANNEL1) == 0) {
				led_value = ~led_value;
			}
			key_press = 0;
			XGpio_InterruptClear(&AXI_Gpio, 0x00000001);
			XGpioPs_WritePin(&Gpio, MIO10_LED, led_value);
			usleep(200000);
			XGpio_InterruptEnable(&AXI_Gpio, 0x00000001);
		}
	}
	return 0;
}

void SetupInterruptSystem(XScuGic *GicInstancePtr, XGpio *AXI_Gpio,
				u16 AXI_GpioIntrId)
{
	//
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	XScuGic_CfgInitialize(GicInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);

	//
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				GicInstancePtr);

	//
	XScuGic_Connect(GicInstancePtr, AXI_GPIO_INTERRUPT_ID,
				(Xil_ExceptionHandler)IntrHandler,
				(void *)AXI_Gpio);
	//
	XScuGic_Enable(GicInstancePtr, AXI_GPIO_INTERRUPT_ID);
	// high valid level-sensitive
	XScuGic_SetPriorityTriggerType(GicInstancePtr, AXI_GPIO_INTERRUPT_ID,
						0xA0, 0x1);
	//enable global interrupt
	XGpio_InterruptGlobalEnable(AXI_Gpio);
	//enable bit interrupt
	XGpio_InterruptEnable(AXI_Gpio, 0x00000001);
	//
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
}

void IntrHandler() {
	printf("Interrupt Detected\n\r");
	key_press =1;
	XGpio_InterruptDisable(&AXI_Gpio, 0x00000001);
}

