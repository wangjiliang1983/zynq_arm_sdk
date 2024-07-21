#include <stdio.h>
#include "xparameters.h"
#include "xadcps.h"
#include "sleep.h"

#define XADC_DEVICE_ID 		XPAR_XADCPS_0_DEVICE_ID

XAdcPs XAdcInst;

int XAdcFractionToInt(float FloatNum)
{
	float Temp;

	Temp = FloatNum;
	if (FloatNum < 0) {
		Temp = -(FloatNum);
	}

	return( ((int)((Temp -(float)((int)Temp)) * (1000.0f))));
}



int main () {

	XAdcPs_Config *ConfigPtr;
	u32 TempRawData;
	u32 VccPintRawData;
	u32 VccPauxRawData;
	u32 VccPdroRawData;
	float TempData;
	float VccPintData;
	float VccPauxData;
	float MaxData;
	float MinData;

	printf("\r\nEntering the XAdc Polled Example. \r\n");

	// Initialize the XAdc
	ConfigPtr = XAdcPs_LookupConfig(XADC_DEVICE_ID);
	XAdcPs_CfgInitialize(&XAdcInst, ConfigPtr, ConfigPtr->BaseAddress);

	// Disable the Channel Sequencer before configuring the Sequence registers.
	XAdcPs_SetSequencerMode(&XAdcInst, XADCPS_SEQ_MODE_SAFE);

	while(1) {
	// Read the on-chip Temperature Data (Current/Maximum/Minimum) from ADC
	TempRawData = XAdcPs_GetAdcData(&XAdcInst, XADCPS_CH_TEMP);
	TempData = XAdcPs_RawToTemperature(TempRawData);
	printf("\r\nThe Current Temperature is %0d.%03d Centigrades.\r\n",
			(int)(TempData), XAdcFractionToInt(TempData));
	TempRawData = XAdcPs_GetMinMaxMeasurement(&XAdcInst, XADCPS_MAX_TEMP);
	MaxData = XAdcPs_RawToTemperature(TempRawData);
	printf("The Maximum Temperature is %0d.%03d Centigrades. \r\n",
			(int)(MaxData), XAdcFractionToInt(MaxData));

	TempRawData = XAdcPs_GetMinMaxMeasurement(&XAdcInst, XADCPS_MIN_TEMP);
	MinData = XAdcPs_RawToTemperature(TempRawData & 0xFFF0);
	printf("The Minimum Temperature is %0d.%03d Centigrades. \r\n",
			(int)(MinData), XAdcFractionToInt(MinData));

	// Read the VccPint Votage Data (Current/Maximum/Minimum) from ADC
	VccPintRawData = XAdcPs_GetAdcData(&XAdcInst, XADCPS_CH_VCCPINT);
	VccPintData = XAdcPs_RawToVoltage(VccPintRawData);
	printf("\r\nThe Current VCCPINT is %0d.%03d Volts. \r\n",
			(int)(VccPintData), XAdcFractionToInt(VccPintData));

	VccPintRawData = XAdcPs_GetMinMaxMeasurement(&XAdcInst,XADCPS_MAX_VCCPINT);
	MaxData = XAdcPs_RawToVoltage(VccPintRawData);
	printf("The Maximum VCCPINT is %0d.%03d Volts. \r\n",
			(int)(MaxData), XAdcFractionToInt(MaxData));

	VccPintRawData = XAdcPs_GetMinMaxMeasurement(&XAdcInst,XADCPS_MIN_VCCPINT);
	MinData = XAdcPs_RawToVoltage(VccPintRawData);
	printf("The Minimum VCCPINT is %0d.%03d Volts. \r\n",
			(int)(MinData), XAdcFractionToInt(MinData));

	// Read the VccPaux Votage Data (Current/Maximum/Minimum) from ADC
	VccPauxRawData = XAdcPs_GetAdcData(&XAdcInst, XADCPS_CH_VCCPAUX);
	VccPauxData = XAdcPs_RawToVoltage(VccPauxRawData);
	printf("\r\nThe Current VCCPAUX is %0d.%03d Volts. \r\n",
			(int)(VccPauxData), XAdcFractionToInt(VccPauxData));

	VccPauxRawData = XAdcPs_GetMinMaxMeasurement(&XAdcInst,XADCPS_MAX_VCCPAUX);
	MaxData = XAdcPs_RawToVoltage(VccPauxRawData);
	printf("The Maximum VCCPAUX is %0d.%03d Volts. \r\n",
			(int)(MaxData), XAdcFractionToInt(MaxData));

	VccPauxRawData = XAdcPs_GetMinMaxMeasurement(&XAdcInst,XADCPS_MIN_VCCPAUX);
	MinData = XAdcPs_RawToVoltage(VccPauxRawData);
	printf("The Minimum VCCPAUX is %0d.%03d Volts. \r\n\r\n",
			(int)(MinData), XAdcFractionToInt(MinData));

	// Read the VccPdro Votage Data (Current/Maximum/Minimum) from ADC
	VccPdroRawData = XAdcPs_GetAdcData(&XAdcInst, XADCPS_CH_VCCPDRO);
	VccPintData = XAdcPs_RawToVoltage(VccPdroRawData);
	printf("\r\nThe Current VCCPDDRO is %0d.%03d Volts. \r\n",
			(int)(VccPintData), XAdcFractionToInt(VccPintData));

	VccPdroRawData = XAdcPs_GetMinMaxMeasurement(&XAdcInst,XADCPS_MAX_VCCPDRO);
	MaxData = XAdcPs_RawToVoltage(VccPdroRawData);
	printf("The Maximum VCCPDDRO is %0d.%03d Volts. \r\n",
			(int)(MaxData), XAdcFractionToInt(MaxData));

	VccPdroRawData = XAdcPs_GetMinMaxMeasurement(&XAdcInst,XADCPS_MIN_VCCPDRO);
	MinData = XAdcPs_RawToVoltage(VccPdroRawData);
	printf("The Minimum VCCPDDRO is %0d.%03d Volts. \r\n",
			(int)(MinData), XAdcFractionToInt(MinData));

	sleep(5);
	}

	printf("Exiting the XAdc Polled Example. \r\n");
	return 0;
}
