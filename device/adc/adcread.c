#include <xinu.h>

void adc_init(){
	uint32 *adc_tsc_clkptr, *ctrlptr;

	adc_tsc_clkptr = (uint32*)(0x44E004BC);
	if(DEBUG) kprintf("Reading WKUP_ADC_TSC_CLKCTRL register value: %d\n", *adc_tsc_clkptr);
	*adc_tsc_clkptr |= 0x02;
	sleep(1);
	if(DEBUG) kprintf("Updated WKUP_ADC_TSC_CLKCTRL register value: %d\n", *adc_tsc_clkptr);
	
	ctrlptr = (uint32*)(ADC_CTRL);
	// ctrl pointer set
	// write protection is off
	if(DEBUG) kprintf("Reading ADC_CTRL register value: %d\n", *ctrlptr);
	*ctrlptr |= (0x01<<2);
	if(DEBUG) kprintf("Updated ADC_CTRL register value: %d\n", *ctrlptr);
	
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
	
	if(DEBUG) kprintf("Stepconfig 1 : %d\n", *(uint32*)ADCSTEPCONFIG1);
	if(DEBUG) kprintf("Stepdelay 1 : %d\n", *(uint32*)ADCSTEPDELAY1);
	//updated all stepconfigs and step delays

	//clear out fifo data
	int output;
	while(*((uint32*)FIFO0COUNT) & FIFO_COUNT_MASK){
		output =  *(uint32*)ADC_FIFO0DATA & ADC_FIFO_MASK;
	}
	
	//enable the adc
	*ctrlptr |= 0x01;
}

devcall adc_read(int32 blk) {
	kprintf("debug  %d\n",DEBUG);
	// adc_init();
	if(DEBUG == 1) kprintf("ADC STEPENABLE : %d\n", *(uint32 *)ADC_STEPENABLE);
	int refvol_const = 1800;
	*(uint32*)ADC_STEPENABLE |= blk;
	
	kprintf("FIFO0 Count before loop :  %d\n", *(uint32*)FIFO0COUNT);
	while(!(*(uint32*)FIFO0COUNT & FIFO_COUNT_MASK)){}
	kprintf("FIFO0 Count after loop & before reading :  %d\n", *(uint32*)FIFO0COUNT);
	int32 val = *(uint32 *)ADC_FIFO0DATA & ADC_FIFO_MASK;
	float voltage_reading = (val*refvol_const)/4095;	
	kprintf("FIFO0 Count after reading :  %d\n", *(uint32*)FIFO0COUNT);
	kprintf("Pin reading value :%d\n", val);
	return (int32)voltage_reading;
}
