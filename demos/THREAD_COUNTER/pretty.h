#ifndef __PRETTY_H_
#define __PRETTY_H_

#define CSIESC  0x1B
#define STDOUT	(BaseSequentialStream *)&SD2
/* Puts the cursor at the top of the current
 * available screen area
 */
int puttop(void);

/* Moves the cursor vertically by given number */
int movv(int);

/* Moves the cursor horizontally by given number.*/
int movh(int);

/* Clears the screen */
int cls(void);


/* Puts the cursor at the given row or column */
int putcursor(int, int);


/* Clears the line to the right beginning at the cursor */
int clclr(void);

int set_text_color(int);

int unset_text_color(int);

#endif
