#include <xinu.h>
int32 pread(int32 dev_id){
	switch (dev_id)
	{
		case 11 : 
			return adc_read(11);
			break;
		default:
			return -1;
	}
}
int32 pwrite(int32 dev_id, int32 data){
	switch (dev_id)
	{
		case 7 : 
			gpio_write(7, data);
			break;
		default:
			break;
	}
	return OK; }
