#include <stdio.h>
#include "xil_printf.h"
#include "xbram_hw.h"
#include "pl_bram_rd.h"
#include "xparameters.h"

#define PL_BRAM_START		PL_BRAM_RD_S00_AXI_SLV_REG0_OFFSET
#define PL_BRAM_START_ADDR	PL_BRAM_RD_S00_AXI_SLV_REG1_OFFSET
#define PL_BRAM_LEN			PL_BRAM_RD_S00_AXI_SLV_REG2_OFFSET
#define PL_BRAM_BASE		XPAR_PL_BRAM_RD_0_S00_AXI_BASEADDR

#define START_ADDR			0
#define BRAM_DATA_BYTE		4

char ch_data[1024];
int  ch_data_len;

int main() {

	while(1){
		int wr_cnt = 0;
		int rd_data = 0;
		printf("Please input data to read and write BRAM\n");
		scanf("%1024s",ch_data);
		ch_data_len = strlen(ch_data);
		for(int i=START_ADDR*BRAM_DATA_BYTE;  i<(START_ADDR+ch_data_len)*BRAM_DATA_BYTE;i+=BRAM_DATA_BYTE) {
			XBram_WriteReg(XPAR_BRAM_0_BASEADDR,i,ch_data[wr_cnt]);
			wr_cnt++;
		}

		//configure the start address in PL_BRAM_RD
		PL_BRAM_RD_mWriteReg(PL_BRAM_BASE, PL_BRAM_START_ADDR, START_ADDR*BRAM_DATA_BYTE);
		PL_BRAM_RD_mWriteReg(PL_BRAM_BASE, PL_BRAM_LEN, ch_data_len*BRAM_DATA_BYTE);
		PL_BRAM_RD_mWriteReg(PL_BRAM_BASE, PL_BRAM_START, 0xFFFFFFFF);
		PL_BRAM_RD_mWriteReg(PL_BRAM_BASE, PL_BRAM_START, 0x00000000);

		for(int i=START_ADDR*BRAM_DATA_BYTE;  i<(START_ADDR+ch_data_len)*BRAM_DATA_BYTE;i+=BRAM_DATA_BYTE) {
			rd_data = XBram_ReadReg(XPAR_BRAM_0_BASEADDR,i);
			printf("BRAM address is %d\t, read data is %c\n",i/BRAM_DATA_BYTE,rd_data);
		}
	}

	return 0;
}
