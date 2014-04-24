/** Program to flash the LEDs at different rate * and output message. Four worker threads track
 * count and output message over serial port.
 * Author : Suraj Deuja
 * 
 * To build the file drop it to the demo
 * directory and then run make
 */
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "pretty.h"
#include <string.h>

union heap {
	struct {
		WORKING_AREA(T0, 128);
		WORKING_AREA(T1, 128);
		WORKING_AREA(T2, 128);
		WORKING_AREA(T3, 128);
		WORKING_AREA(T4, 128);
	} wa;
	uint8_t buffer[THD_WA_SIZE(128)*5];
};

//static union heap queueHeap; 
uint8_t buff[32];


void print_msg(GenericQueue *qp){
	(void)*qp;
	chprintf(STDOUT, "queue full\r\n");
}

OUTPUTQUEUE_DECL(OQUEUE, buff, 4, print_msg, NULL);

int main(void) {
	halInit();
	chSysInit();

	chOQInit(&OQUEUE, buff, 4, print_msg, NULL);
	
	/* Activates the UART driver 2, PA2(TX) and PA3(RX). */
	sdStart(&SD2,NULL);
	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

	palSetPad(GPIOD, GPIOD_LED3);
	cls();
	puttop();

	chprintf(STDOUT,"Welcome to ChibiOS-%s\r\n",CH_KERNEL_VERSION);

	movv(19);
	chprintf(STDOUT, "Thread Information\r\n");
	chprintf(STDOUT, "THREAD NAME\t\t\tTHREAD PRIO\tTHREAD TICKS\tTHREAD_STATE");
	

	return 0;
}

/* vim :set tabstop=8 */
