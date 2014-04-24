#include "hal.h"
#include "ch.h"
#include "ff.h"
#include "chprintf.h"


static uint8_t inbuf[23];
static FATFS SDC_FS;

static volatile bool_t fs_ready = FALSE;

#define stdout (BaseSequentialStream *)&SD2

void read_sdcard(void)
{
	if(!sdcConnect(&SDCD1)){
		chprintf(stdout,"OK\r\n");
		chprintf(stdout,"SDCARD CONTENT ");
		chprintf(stdout, "%X %X %X %X\r\n", (&SDCD1)->csd[3],(&SDCD1)->csd[2],
						    (&SDCD1)->csd[1],(&SDCD1)->csd[0]);
	}	

	/* Do something with file */
	FRESULT err;
	err = f_mount(0, &SDC_FS);
	if (err != FR_OK) {
		chprintf(stdout,"Error: Could not mount sdcard\r\n");
	} else {
		chprintf(stdout,"Success: Mounted sdcard\r\n");
		fs_ready = TRUE;	
	}

	/* Open file for reading */
	FIL FileObject;
	err = f_open(&FileObject, "myfile.txt", FA_READ);
	if (err != FR_OK) {
		chprintf(stdout,"Error: Could not openfile\r\n");
	} else {
		chprintf(stdout, "Success: Opened file\r\n");
	}

	/* Lets read the file */
	char buff[8];
	buff[7] = '\0';
	unsigned int bytesRead = 1;
	while(FR_OK == f_read(&FileObject,&buff,7,&bytesRead) && bytesRead > 0) {
		chprintf(stdout, "%s",buff);
	}	
	f_close(&FileObject);
	sdcDisconnect(&SDCD1);
	palSetPad(GPIOD,GPIOD_LED4);
}

static const SDCConfig sddcfg = {
	0
};



int main(void) {
	halInit();
	chSysInit();
	
	palSetPad(GPIOD,GPIOD_LED3);

	sdStart(&SD2, NULL);

    	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
    	palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

	sdcStart(&SDCD1,NULL);
	read_sdcard();

	while(1);
	return 0;
}

