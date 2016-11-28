#include <xinu.h>

devcall gpio_write(int32 pinno, int32 blk) {

	*(uint32*)GPIO_OUTPUT_ENABLE &= (0xFFFFFFFF ^ (1 << pinno));
	if(blk!=0)
		*(uint32*)GPIO_DATA_OUT |= (1 << pinno);
	else
		*(uint32*)GPIO_DATA_OUT &= (0xFFFFFFFF ^ (1 << pinno));
		
	//gptr->setdataout = 0xFFFFFFFF;
	kprintf("Output Enable register value: %d", *(uint32*)GPIO_OUTPUT_ENABLE);
	kprintf("Data out register value: %d", *(uint32*)GPIO_DATA_OUT);
	return OK;
}
