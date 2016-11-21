#include <xinu.h>

devcall adcread(struct dentry *devptr,
    		  char *buff, 
    		  int32 blk) {
	
	//struct adc_csreg * adcptr;
	uint32 * adcptr;
	//adcptr = (struct adc_csreg *)devptr->dvcsr;
	//adcptr = (uint32 *)();
	//gptr->oer &= (0xFFFFFFFF ^ (1 << 28));
        //gptr->data_out |= (1 << 28);
	adcptr = (uint32*)(STEP_ENABLE);
	*adcptr |= (1 << 2);
	kprintf("Reached adc read: %d", *adcptr);
	return 1;
}
