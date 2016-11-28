#include <xinu.h>

devcall gpioinit(struct dentry *devptr){
    
	// struct gpio_csreg * gptr;
	// gptr = (struct gpio_csreg *)devptr->dvcsr;

   	//gptr->oer = 0xEFFFFFFF;
    	//gptr->oer = 0xFFFFFFFF;
    	return OK;
}
