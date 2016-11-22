#include <xinu.h>

devcall adcread(struct dentry *devptr,
    		  char *buff, 
    		  int32 blk) {
	
	uint32 *adc_tsc_clkptr, *ctrlptr;
	
	adc_tsc_clkptr = (uint32*)(0x44E004BC);
	kprintf("Reading WKUP_ADC_TSC_CLKCTRL register value: %d\n", *adc_tsc_clkptr);
	*adc_tsc_clkptr |= 0x02;
	sleep(1);
	kprintf("Updated WKUP_ADC_TSC_CLKCTRL register value: %d\n", *adc_tsc_clkptr);

	ctrlptr = (uint32*)(ADC_CTRL);
	kprintf("Reading ADC_CTRL register value: %d\n", *ctrlptr);
	*ctrlptr |= (0x01<<2);
	kprintf("Updated ADC_CTRL register value: %d\n", *ctrlptr);
	
	return 1;
}
