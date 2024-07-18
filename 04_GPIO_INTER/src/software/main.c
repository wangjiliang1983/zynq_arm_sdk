#include <stdio.h>
#include "xparameters.h"
#include "xgpiops.h"
#include "xscugic.h"

#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID
//GPIO的终端号为52
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID

#define GPIO_INTERRUPT_ID	XPAR_XGPIOPS_0_INTR
#define MIO14_KEY  14
#define MIO10_LED  10
#define GPIO_BANK	XGPIOPS_BANK0  /* Bank 0 of the GPIO Device */

static XGpioPs Gpio; /* The Instance of the GPIO Driver */
static XScuGic Intc; /* The Instance of the Interrupt Controller Driver */

XGpioPs_Config *ConfigPtr;
XScuGic_Config *IntcConfig; /* Instance of the interrupt controller */

void SetupInterruptSystem(XScuGic *GicInstancePtr, XGpioPs *Gpio, u16 GpioIntrId);
//void IntrHandler(void *CallBackRef, u32 Bank, u32 Status);
void IntrHandler();

int main(void) {
	printf("GPIO MIO INTERRUPT TEST!\n\r");

	//初始化GPIO
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);

	//将PS按键的GPIO的方向设置为输入
	XGpioPs_SetDirectionPin(&Gpio, MIO14_KEY, 0x0);

	//将PS LED的GPIO方向设置为输出
	XGpioPs_SetDirectionPin(&Gpio, MIO10_LED, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, MIO10_LED, 1);

	//设置中断系统
	SetupInterruptSystem(&Intc, &Gpio, GPIO_INTERRUPT_ID);
	while(1){

	}
	return 0;
}

void SetupInterruptSystem(XScuGic *GicInstancePtr, XGpioPs *Gpio,
				u16 GpioIntrId)
{
	//查找GIC器件配置信息，并进行初始化
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	XScuGic_CfgInitialize(GicInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);

	//初始化ARM处理器异常句柄
	Xil_ExceptionInit();
	//来给IRQ异常注册处理程序
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				GicInstancePtr);

	//关联中断处理函数
	XScuGic_Connect(GicInstancePtr, GpioIntrId,
				(Xil_ExceptionHandler)IntrHandler,
				(void *)Gpio);
	//GIC中GPIO器件使能中断
	XScuGic_Enable(GicInstancePtr, GpioIntrId);

	//设置MIO引脚中断触发类型，下降沿
	XGpioPs_SetIntrTypePin(Gpio,MIO14_KEY,XGPIOPS_IRQ_TYPE_EDGE_FALLING);

	//设置打开MIO引脚中断使能信号
	XGpioPs_IntrEnablePin(Gpio, MIO14_KEY);


	//使能处理器中断
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
}

void IntrHandler() {
	printf("Interrupt Detected\n\r");
	XGpioPs_IntrDisablePin(&Gpio, MIO14_KEY);
}

