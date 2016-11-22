struct  adc_csreg
{
        volatile uint32 data1;
};

/* Definintion of individual bits in control and status registers       */
#define WKUP_ADC_TSC_CLKCTRL	0x44E004BC
#define ADC_CTRL		0x44E0D040
#define STEP_ENABLE		0x44E0D054
#define STEP_CONFIG		0x44E0D06C
#define STEP_DELAY		0x44E0D070
