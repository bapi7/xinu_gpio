#include <xinu.h>

devcall gpiowrite(struct dentry *devptr,
    		  char *buff, 
    		  int32 blk) {

	struct gpio_csreg * gptr;
	gptr = (struct gpio_csreg *)devptr->dvcsr;
	gptr->oer &= (0xFFFFFFFF ^ (1 << 28));
	gptr->data_out |= (1 << 28); 
	//gptr->setdataout = 0xFFFFFFFF;
	kprintf("Reached gpio write oer: %d", gptr->oer);
	kprintf("Reached gpio write data_out : %d", gptr->data_out);
	return OK;
}
