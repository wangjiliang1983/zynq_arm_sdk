#include "xparameters.h"
#include "breath_led_ip.h"
#include <stdio.h>
#include "sleep.h"
#include "xil_io.h"

#define LED_IP_BAEADDR 	XPAR_BREATH_LED_IP_0_S00_AXI_BASEADDR
#define LED_IP_REG0 	BREATH_LED_IP_S00_AXI_SLV_REG0_OFFSET
#define LED_IP_REG1 	BREATH_LED_IP_S00_AXI_SLV_REG1_OFFSET

int main() {
	printf("Breath LED TEST!\n");
	int freq_flag = 0;
	int led_status = 0;
	while(1) {
		//
		led_status = BREATH_LED_IP_mReadReg(LED_IP_BAEADDR, LED_IP_REG0);
		// if led is on
		if (led_status == 0x1) {
			printf("Breath LED ON!\n");
			sleep(5);
			BREATH_LED_IP_mWriteReg(LED_IP_BAEADDR, LED_IP_REG0, 0x00000000);
		} else {
			if(freq_flag==0) {
				BREATH_LED_IP_mWriteReg(LED_IP_BAEADDR, LED_IP_REG1, 0x800000ef);
				freq_flag = 1;
			} else {
				BREATH_LED_IP_mWriteReg(LED_IP_BAEADDR, LED_IP_REG1, 0x8000002f);
				freq_flag = 0;
			}
			printf("Breath LED OFF!\n");
			sleep(1);
			BREATH_LED_IP_mWriteReg(LED_IP_BAEADDR, LED_IP_REG0, 0x00000001);
		}
	}
	return 0;
}
