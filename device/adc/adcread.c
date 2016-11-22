#include <xinu.h>

void adcinittt(){
	uint32 *adc_tsc_clkptr, *ctrlptr;

	adc_tsc_clkptr = (uint32*)(0x44E004BC);
	kprintf("Reading WKUP_ADC_TSC_CLKCTRL register value: %d\n", *adc_tsc_clkptr);
	*adc_tsc_clkptr |= 0x02;
	sleep(1);
	kprintf("Updated WKUP_ADC_TSC_CLKCTRL register value: %d\n", *adc_tsc_clkptr);
	
	ctrlptr = (uint32*)(ADC_CTRL);
	// ctrl pointer set
	// write protection is off
	kprintf("Reading ADC_CTRL register value: %d\n", *ctrlptr);
	*ctrlptr |= (0x01<<2);
	kprintf("Updated ADC_CTRL register value: %d\n", *ctrlptr);
	
	*(uint32*)ADCSTEPCONFIG1 |= 0x00<<19 | 0b000<<2 | 0b00;
	*(uint32*)ADCSTEPDELAY1 |= 0<<24;
	*(uint32*)ADCSTEPCONFIG2 |= 0x01<<19 | 0b000<<2 | 0b00;
	*(uint32*)ADCSTEPDELAY2 |= 0<<24;
	*(uint32*)ADCSTEPCONFIG3 |= 0x02<<19 | 0b000<<2 | 0b00;
	*(uint32*)ADCSTEPDELAY3 |= 0<<24;
	*(uint32*)ADCSTEPCONFIG4 |= 0x03<<19 | 0b000<<2 | 0b00;
	*(uint32*)ADCSTEPDELAY4 |= 0<<24;
	*(uint32*)ADCSTEPCONFIG5 |= 0x04<<19 | 0b000<<2 | 0b00;
	*(uint32*)ADCSTEPDELAY5 |= 0<<24;
	*(uint32*)ADCSTEPCONFIG6 |= 0x05<<19 | 0b000<<2 | 0b00;
	*(uint32*)ADCSTEPDELAY6 |= 0<<24;
	*(uint32*)ADCSTEPCONFIG7 |= 0x06<<19 | 0b000<<2 | 0b00;
	*(uint32*)ADCSTEPDELAY7  |= 0<<24;
	*(uint32*)ADCSTEPCONFIG8 |= 0x07<<19 | 0b000<<2 | 0b00;
	*(uint32*)ADCSTEPDELAY8  |= 0<<24;
	
	kprintf("Stepconfig 2 : %d\n", *(uint32*)ADCSTEPCONFIG2);
	kprintf("Stepdelay 2 : %d\n", *(uint32*)ADCSTEPDELAY2);
	//updated all stepconfigs and step delays

	//clear out fifo data
	int output;
	kprintf("FIFO Out before :  %d\n", *(uint32*)ADC_FIFO0DATA);
	while(*((uint32*)FIFO0COUNT) & FIFO_COUNT_MASK){
		output =  *(uint32*)ADC_FIFO0DATA & ADC_FIFO_MASK;
	}
	kprintf("FIFO Out after :  %d\n", *(uint32*)ADC_FIFO0DATA);
	
	// just suppress the warning about output not being used
	if(output){}
	
	//enable the adc
	*ctrlptr |= 0x01;
}

devcall adcread(struct dentry *devptr,
    		  char *buff, 
    		  int32 blk) {
	//enable a step
	adcinittt();
	//TODO : make this pin configurable
	//setting the first bit to one
	*(uint32*)ADC_STEPENABLE |= (0b1)<< 1;
	//print the output
	kprintf("FIFO Out 0 before :  %d\n", *(uint32*)ADC_FIFO0DATA);
	kprintf("FIFO Out 1 before :  %d\n", *(uint32*)ADC_FIFO1DATA);
	while(!(*(uint32*)FIFO0COUNT & FIFO_COUNT_MASK)){}
	int32 val = *(uint32 *)ADC_FIFO0DATA & ADC_FIFO_MASK;

	kprintf("Data output val :%d\n", val);
	kprintf("Data output val converted :%f\n", val*1.8/4095.0);
	return 1;
}
