#include "stdio.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "xil_io.h"

int main() {

	int i;
	char c;

	Xil_DCacheDisable();
	printf("AXI4 PL RW PS DDR TEST\n\r");
	while(1) {
		scanf("%c", &c);
		if (c== 'c') {
			printf("Start\n\r");
			for(i=0;i<4096;i=i+4) {
				printf("%d is %d\n\r",i,(int)(Xil_In32(0x10000000+i)));
			}
		}
	}
	return 0;
}
