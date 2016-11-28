#include <xinu.h>
int32 pread(int32 dev_id){
	switch (dev_id)
	{
		case 1 : 
			return adc_read(11);
			break;
		default:
			return -1;
	}
}
int32 pwrite(int32 dev_id, int32 data){
	switch (dev_id)
	{
		case 0 : 
			gpio_write(16, data);
			break;
		default:
			break;
	}
	return OK; }
