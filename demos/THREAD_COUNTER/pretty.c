/*
 * Defines the functions declared in the header pretty.h
 * Author : Suraj Deuja
 * Email : deujasuraj77@gmail.com
 */
#include "pretty.h"
#include "ch.h"
#include "hal.h"
#include "chprintf.h"

/* Puts the cursor at the top of the current
 * available screen area
 */
int puttop()
{
	chprintf(STDOUT, "%c[H", CSIESC);
	return 0;
}

/* Moves the cursor vertically by given number */
int movv(int row)
{
	if (row < 0)
		chprintf(STDOUT,"%c[%dA", CSIESC, row);
	else
		chprintf(STDOUT,"%c[%dB", CSIESC, row);
	return 0;
}

/* Moves the cursor horizontally by given number.*/
int movh(int col)
{
	if (col < 0)
		chprintf(STDOUT,"%c[%dD", CSIESC, col);
	else
		chprintf(STDOUT,"%c[%dC", CSIESC, col);
	return 0;
}

/* Clears the screen */
int cls()
{
	chprintf(STDOUT, "%c[2J", CSIESC);
	return 0;
}

/* Puts the cursor at the given row or column */
int putcursor(int row, int col)
{
	chprintf(STDOUT, "%c[%d;%dH", CSIESC, row-1, col);
	chprintf(STDOUT, "\n");
	return 0;
}

/* Clears the line to the right beginning at the cursor */
int clclr()
{
	chprintf(STDOUT, "%c[0K", CSIESC);
	return 0;
}

int set_text_color(int color)
{
	chprintf(STDOUT, "%c[%dm", CSIESC, color);
	return 0;
}
int unset_text_color(int color)
{
	chprintf(STDOUT, "%c[2%dm", CSIESC, color);
	return 0;
}
