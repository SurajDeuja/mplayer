/** Program to flash the LEDs at different rate
 * and output message. Four worker threads track
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

enum THDNAME  {THDORANGE, THDGREEN, THDBLUE, THDRED, MAIN};

/* Define a mutex for writing to serial. */
static Mutex mutex_thmsg;
static Thread * THREAD_POINTERS[] = { 0, 0, 0, 0, 0};

static WORKING_AREA(waThread1, 512);
static WORKING_AREA(waThread2, 512);
static WORKING_AREA(waThread3, 512);
static WORKING_AREA(waThread4, 512);

/* GCC specific attribute. This tells the compiler
 * that the threads do not return. Otherwise compiler
 * emits warning.
 */
static msg_t thread1(void *arg) __attribute__((noreturn));
static msg_t thread2(void *arg) __attribute__((noreturn));
static msg_t thread3(void *arg) __attribute__((noreturn));
static msg_t thread4(void *arg) __attribute__((noreturn));

void print_msg(enum THDNAME, char *, int, int);
void print_num(enum THDNAME, int, int, int);
char * getThreadStateName(Thread *);
void updatePrioTable(void);
void updateThdTicksTable(void);

static msg_t thread1(void *arg) {
	(void)arg;
	chRegSetThreadName("Orange blinker");
	enum THDNAME thd_name = THDORANGE;
	unsigned int count = 0;
	THREAD_POINTERS[0] = currp;
	while(1) {
		palSetPad(GPIOD, GPIOD_LED3);

		chMtxLock(&mutex_thmsg);
			print_msg(thd_name, "ORANGE:",1, 2);	
			print_num(thd_name, count, 1, 20);	
			print_msg(thd_name, (char *)chRegGetThreadName(currp),22,0);
			print_num(thd_name, chThdGetPriority(), 22, 30); 
			updatePrioTable();
		chMtxUnlock();

		chThdSleepMilliseconds(250);
		palClearPad(GPIOD, GPIOD_LED3);
		chThdSleepMilliseconds(250);
		count++;
	}
}

static msg_t thread2(void *arg) {
	(void)arg;
	chRegSetThreadName("Green blinker");
	enum THDNAME thd_name = THDGREEN;
	THREAD_POINTERS[1] = currp;

	unsigned int count = 0;
	while(1) {
		palSetPad(GPIOD, GPIOD_LED4);

		chMtxLock(&mutex_thmsg);
			print_msg(thd_name, "GREEN:",2, 2);	
			print_num(thd_name, count,2,20);	
			print_msg(thd_name, (char *)chRegGetThreadName(currp),23,0);
			print_num(thd_name, chThdGetPriority(), 23, 30); 
			updatePrioTable();
		chMtxUnlock();

		chThdSleepMilliseconds(500);
		palClearPad(GPIOD, GPIOD_LED4);
		chThdSleepMilliseconds(500);

		count++;
	}
}

static msg_t thread3(void *arg) {
	(void)arg;

	chRegSetThreadName("Red blinker");
	enum THDNAME thd_name = THDRED;
	THREAD_POINTERS[2] = currp;

	unsigned int count = 0;
	while(1) {
		palSetPad(GPIOD, GPIOD_LED5);

		chMtxLock(&mutex_thmsg);
			print_msg(thd_name, "RED:",3, 2);	
			print_num(thd_name, count,3,20);	
			print_msg(thd_name, (char *)chRegGetThreadName(currp),24,0);
			print_num(thd_name, chThdGetPriority(), 24, 30); 
			updatePrioTable();
		chMtxUnlock();

		chThdSleepMilliseconds(1000);
		palClearPad(GPIOD, GPIOD_LED5);
		chThdSleepMilliseconds(1000);

		count++;
	}
}

static msg_t thread4(void *arg) {
	(void)arg;

	chRegSetThreadName("Blue blinker");
	enum THDNAME thd_name = THDBLUE;
	THREAD_POINTERS[3] = currp;

	unsigned int count = 0;
	while(1) {
		palSetPad(GPIOD, GPIOD_LED6);

		chMtxLock(&mutex_thmsg);
			print_msg(thd_name, "BLUE:",4, 2);	
			print_num(thd_name, count,4,20);	
			print_msg(thd_name, (char *)chRegGetThreadName(currp),25,0);
			print_num(thd_name, chThdGetPriority(), 25, 30); 
			updatePrioTable();
			updateThdTicksTable();	
		chMtxUnlock();

		chThdSleepMilliseconds(2000);
		palClearPad(GPIOD, GPIOD_LED6);
		chThdSleepMilliseconds(2000);
		count++;
	}
}

void print_msg(enum THDNAME thd, char *msg, int row, int col) {
	puttop();

	movv(row);
	movh(col);

	switch(thd) {
	case THDORANGE:
		chprintf(STDOUT,msg);
		break;
	case THDBLUE:
		chprintf(STDOUT,msg);
		break;
	case THDRED:
		chprintf(STDOUT,msg);
		break;
	case THDGREEN:
		chprintf(STDOUT,msg);
		break;
	case MAIN:
		chprintf(STDOUT,msg);
		break;

	}

}

void print_num(enum THDNAME thd, int num, int row, int col) {
	puttop();
	movv(row);
	movh(col);

	switch(thd) {
	case THDORANGE:
		chprintf(STDOUT,"%i",num);
		break;
	case THDBLUE:
		chprintf(STDOUT,"%i",num);
		break;
	case THDRED:
		chprintf(STDOUT,"%i",num);
		break;
	case THDGREEN:
		chprintf(STDOUT,"%i",num);
		break;
	case MAIN:
		chprintf(STDOUT,"%i",num);
		break;


	}

}

void updatePrioTable() {
	puttop();
	movv(22);
	movh(55);
	int i;
	for (i = 0; i < 5; i++) {
		if (THREAD_POINTERS[i] != 0) {
			putcursor(23+i,60);
			movh(55);
			chprintf(STDOUT,getThreadStateName(THREAD_POINTERS[i]),"\r\n");
		
		}
	} 
}

void updateThdTicksTable() {
	puttop();
	movv(23);
	movh(45);
	int i;
	for (i = 0; i < 5; i++) {
		if (THREAD_POINTERS[i] != 0) {
			putcursor(23+i,40);
			movh(45);
			chprintf(STDOUT,"%i\r\n",chThdGetTicks(THREAD_POINTERS[i]));
		}
	} 
}

char * getThreadStateName(Thread *tp ){
	switch(tp->p_state) {
	case 0: 
		return "THD_STATE_READY";
	case 1: 
		return "THD_STATE_CURRE";
	case 2:
		return "THD_STATE_SUSPE";
	case 3:
		return "THD_STATE_WTSEM";
	case 4:
		return "THD_STATE_WTMTX";
	case 5:
		return "THD_STATE_WTCON";
	case 6:
		return "THD_STATE_SLEEP";
	case 7:
		return "THD_STATE_WTEXI";
	case 8:
		return "THD_STATE_WTORE";
	case 9:
		return "THD_STATE_WTAND";
	case 10:
		return "THD_STATE_SNDMS";
	case 11:
		return "THD_STATE_WTMSG"; 
	case 12:
		return "THD_STATE_WTQUE";
	case 13:
		return "THD_STATE_FINAL";
	case 14:
		return "THD_STATE_NAMES";
	}
	return 0;
}

int main(void) {
	halInit();
	chSysInit();
	chMtxInit(&mutex_thmsg);

	/* Activates the UART driver 2, PA2(TX) and PA3(RX). */
	sdStart(&SD2,NULL);
	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

	cls();
	puttop();

	chprintf(STDOUT,"Welcome to ChibiOS-%s\r\n",CH_KERNEL_VERSION);

	movv(19);
	chprintf(STDOUT, "Thread Information\r\n");
	chprintf(STDOUT, "THREAD NAME\t\t\tTHREAD PRIO\tTHREAD TICKS\tTHREAD_STATE");
	
	THREAD_POINTERS[4] = currp;
	enum THDNAME thd_name = MAIN;

	
	/* Creates threads */
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, thread1, NULL);
	chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, thread2, NULL);
	chThdCreateStatic(waThread3, sizeof(waThread3), NORMALPRIO, thread3, NULL);
	chThdCreateStatic(waThread4, sizeof(waThread4), NORMALPRIO, thread4, NULL);
	
	while(1) {
		chMtxLock(&mutex_thmsg);
			print_msg(thd_name, "Main", 26, 0);
			print_num(thd_name, chThdGetPriority(), 26, 30); 
			updatePrioTable();
		updateThdTicksTable();

		chMtxUnlock();
	}
		//chprintf(STDOUT,"%i", (int)chThdGetTicks(THREAD_POINTERS[0]));
	return 0;
}

/* vim :set tabstop=8 */
