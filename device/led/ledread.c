/* ledread.c - ledread */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ledread  -  Read value from a led device (interrupts disabled)
 *------------------------------------------------------------------------
 */
devcall	ledread(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	)
{
	char	ch;			/* Next input character		*/

	/* Block until input arrives */

	ch = ledgetc(devptr);

	return ch;
}
