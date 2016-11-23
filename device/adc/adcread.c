#include <xinu.h>

void adc_init(){
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
	
	kprintf("Stepconfig 1 : %d\n", *(uint32*)ADCSTEPCONFIG1);
	kprintf("Stepdelay 1 : %d\n", *(uint32*)ADCSTEPDELAY1);
	//updated all stepconfigs and step delays

	//clear out fifo data
	int output;
	while(*((uint32*)FIFO0COUNT) & FIFO_COUNT_MASK){
		output =  *(uint32*)ADC_FIFO0DATA & ADC_FIFO_MASK;
	}
	
	//enable the adc
	*ctrlptr |= 0x01;
}

devcall adcread(struct dentry *devptr,
    		  char *buff, 
    		  int32 blk) {
	adc_init();
	int refvol_const = 1800;
	*(uint32*)ADC_STEPENABLE |= 0x04;
	
	kprintf("FIFO0 Count before loop :  %d\n", *(uint32*)FIFO0COUNT);
	while(!(*(uint32*)FIFO0COUNT & FIFO_COUNT_MASK)){}
	kprintf("FIFO0 Count after loop :  %d\n", *(uint32*)FIFO0COUNT);
	int32 val = *(uint32 *)ADC_FIFO0DATA & ADC_FIFO_MASK;
	float voltage_reading = (val*refvol_const)/4095;	
	kprintf("Pin reading value :%d\n", val);
	return (int32)voltage_reading;
}
