#include <stdio.h>
#include "xparameters.h"
#include "xgpiops.h"
#include "xscugic.h"

#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID
//GPIO���ն˺�Ϊ52
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

	//��ʼ��GPIO
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);

	//��PS������GPIO�ķ�������Ϊ����
	XGpioPs_SetDirectionPin(&Gpio, MIO14_KEY, 0x0);

	//��PS LED��GPIO��������Ϊ���
	XGpioPs_SetDirectionPin(&Gpio, MIO10_LED, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, MIO10_LED, 1);

	//�����ж�ϵͳ
	SetupInterruptSystem(&Intc, &Gpio, GPIO_INTERRUPT_ID);
	while(1){

	}
	return 0;
}

void SetupInterruptSystem(XScuGic *GicInstancePtr, XGpioPs *Gpio,
				u16 GpioIntrId)
{
	//����GIC����������Ϣ�������г�ʼ��
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	XScuGic_CfgInitialize(GicInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);

	//��ʼ��ARM�������쳣���
	Xil_ExceptionInit();
	//����IRQ�쳣ע�ᴦ�����
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				GicInstancePtr);

	//�����жϴ�����
	XScuGic_Connect(GicInstancePtr, GpioIntrId,
				(Xil_ExceptionHandler)IntrHandler,
				(void *)Gpio);
	//GIC��GPIO����ʹ���ж�
	XScuGic_Enable(GicInstancePtr, GpioIntrId);

	//����MIO�����жϴ������ͣ��½���
	XGpioPs_SetIntrTypePin(Gpio,MIO14_KEY,XGPIOPS_IRQ_TYPE_EDGE_FALLING);

	//���ô�MIO�����ж�ʹ���ź�
	XGpioPs_IntrEnablePin(Gpio, MIO14_KEY);


	//ʹ�ܴ������ж�
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
}

void IntrHandler() {
	printf("Interrupt Detected\n\r");
	XGpioPs_IntrDisablePin(&Gpio, MIO14_KEY);
}

