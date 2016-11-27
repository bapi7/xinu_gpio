#include <xinu.h>

devcall gpio_write(int32 pinno, int32 blk) {

	*(uint32*)GPIO1_OUTPUT_ENABLE &= (0xFFFFFFFF ^ (1 << 28));
	if(blk!=0)
		*(uint32*)GPIO1_DATA_OUT |= (1 << 28);
	else
		*(uint32*)GPIO1_DATA_OUT &= (0xFFFFFFFF ^ (1 << 28));
		
	//gptr->setdataout = 0xFFFFFFFF;
	kprintf("Output Enable register value: %d", *(uint32*)GPIO1_OUTPUT_ENABLE);
	kprintf("Data out register value: %d", *(uint32*)GPIO1_DATA_OUT);
	return OK;
}
