/* GPIO memory-mapped I/O address */
#define GPIO1 0x4804C134 //# gpio 1

#define GPIO1_OUTPUT_ENABLE (GPIO1+0x134)
#define GPIO1_DATA_IN (GPIO1+0x138)
#define GPIO1_DATA_OUT (GPIO1+0x13C)
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
