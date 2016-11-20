/* GPIO memory-mapped I/O address */
//#define GPIO_P9_12 0x44e10878 //# gpio 1

//#define GPIO1_DATA_OUT 0x4804c13c
//#define GPIO1_DIR_OUT  0x4804c134

struct  gpio_csreg
{
        //volatile uint32 extra[15];
	volatile uint32 oer;
	volatile uint32 data_in;
	volatile uint32 data_out;
	//volatile uint32 end[6];
	//volatile uint32 cleardataout;
	//volatile uint32 setdataout;
};


#define GPIO1_DATA_OUT 0
