#include "hal.h"
#include "ch.h"
#include "ff.h"
#include "chprintf.h"
#include "wave.h"


static uint8_t buffer[50];

static FATFS SDC_FS;

static volatile bool_t fs_ready = FALSE;

static WAVEFILE *FileWavObject;

#define stdout (BaseSequentialStream *)&SD2

void read_sdcard(void)
{
	if(!sdcConnect(&SDCD1)) {
		chprintf(stdout,"OK\r\n");
		chprintf(stdout,"SDCARD CONTENT ");
		chprintf(stdout, "%X %X %X %X\r\n", (&SDCD1)->csd[3],(&SDCD1)->csd[2],
						    (&SDCD1)->csd[1], (&SDCD1)->csd[0]);
	}	

	/* Do something with file */
	FRESULT err;
	err = f_mount(0, &SDC_FS);
	if (err != FR_OK) {
		chprintf(stdout,"Error: Could not mount sdcard\r\n");
		sdcDisconnect(&SDCD1);
		return;
	} else {
		chprintf(stdout,"Success: Mounted sdcard\r\n");
		fs_ready = TRUE;	
	}

	/* Open file for reading */
	FIL FileObject;
	err = f_open(&FileObject, "dylan.wav", FA_READ);
	if (err != FR_OK) {
		chprintf(stdout,"Error: Could not openfile\r\n");
		sdcDisconnect(&SDCD1);
		return;
	} else {
		chprintf(stdout, "Success: Opened file\r\n");
	}

	/* Lets read the file */
	unsigned int bytesRead = 1;
	err = f_read(&FileObject,&buffer,50,&bytesRead);
	if (err != FR_OK) {
		chprintf(stdout,"unable to read file\r\n");
		f_close(&FileObject);
		sdcDisconnect(&SDCD1);
		return;
	}
	
	/* Extract the header information */
	FileWavObject = (WAVEFILE *)&buffer[0];
	if (FileWavObject->riffSignature == WAVE_HEAD_RIFF 
					 && FileWavObject->fmtSignature == WAVE_HEAD_FMT) {
		chprintf(stdout, "WAV FILE FOUND\r\n");
		chprintf(stdout, "Channels : %d\r\n",FileWavObject->numChannels);
		chprintf(stdout, "Sample Rate : %d\r\n",FileWavObject->sampleRate);
		chprintf(stdout, "No. of Bits : %d\r\n",FileWavObject->numBits);
		chprintf(stdout, "length : %x\r\n",FileWavObject->wavLength);
	}
	else {
		chprintf(stdout, "Unknown file !");	
	}

	f_close(&FileObject);
	sdcDisconnect(&SDCD1);
	palSetPad(GPIOD, GPIOD_LED4);
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

