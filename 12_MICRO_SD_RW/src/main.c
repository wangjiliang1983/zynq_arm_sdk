#include "xparameters.h"
#include "xil_printf.h"
#include "ff.h"
#include "xdevcfg.h"

#define FILE_NAME "ZDYZ.txt"

const char src_str[30] = "www.openedv.com";
static FATFS fatfs;

int platform_init_fs()
{
	FRESULT status;
	TCHAR *Path = "0:/";
	BYTE work[FF_MAX_SS];

    status = f_mount(&fatfs, Path, 1);
	if (status != FR_OK) {
		xil_printf("Volume is not FAT formated; formating FAT\r\n");

		status = f_mkfs(Path, FM_FAT32, 0, work, sizeof work);
		if (status != FR_OK) {
			xil_printf("Unable to format FATfs\r\n");
			return -1;
		}

		status = f_mount(&fatfs, Path, 1);
		if (status != FR_OK) {
			xil_printf("Unable to mount FATfs\r\n");
			return -1;
		}
	}
	return 0;
}

int sd_mount()
{
    FRESULT status;

    status = platform_init_fs();
    if(status){
        xil_printf("ERROR: f_mount returned %d!\n",status);
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}

int sd_write_data(char *file_name,u32 src_addr,u32 byte_len)
{
    FIL fil;
    UINT bw;

    f_open(&fil,file_name,FA_CREATE_ALWAYS | FA_WRITE);
    f_lseek(&fil, 0);
    f_write(&fil,(void*) src_addr,byte_len,&bw);
    f_close(&fil);
    return 0;
}

int sd_read_data(char *file_name,u32 src_addr,u32 byte_len)
{
	FIL fil;
    UINT br;

    f_open(&fil,file_name,FA_READ);
    f_lseek(&fil,0);
    f_read(&fil,(void*)src_addr,byte_len,&br);
    f_close(&fil);
    return 0;
}

int main()
{
    int status,len;
    char dest_str[30] = "";

    status = sd_mount();
    if(status != XST_SUCCESS){
		xil_printf("Failed to open SD card!\n");
		return 0;
    }
    else
        xil_printf("Success to open SD card!\n");

    len = strlen(src_str);
    sd_write_data(FILE_NAME,(u32)src_str,len);
    sd_read_data(FILE_NAME,(u32)dest_str,len);

    if (strcmp(src_str, dest_str) == 0)
    	xil_printf("src_str is equal to dest_str,SD card test success!\n");
    else
    	xil_printf("src_str is not equal to dest_str,SD card test failed!\n");

    return 0;
  }
