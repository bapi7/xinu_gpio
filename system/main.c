/*  main.c  - main */

#include <xinu.h>

pid32 led,temp;

process ledon() {
    char buffer[10] = "";
    write(GPIO, buffer, 10);
    return OK;
}

process tempread() {
	char * sensor_pin;
	int32 millivolts, temp_c, temp_f, reading;
	sensor_pin = "P9_40";
        kprintf("Starting reading sensor data \n");
        read(ADC, sensor_pin, 10);
        /*kprintf("Read value: %d",reading);
        millivolts = reading * 1800;  // 1.8V reference = 1800 mV
        temp_c = (millivolts - 500) / 10;
        temp_f = (temp_c * 9/5) + 32;
        kprintf("mv=%d C=%d F=%d", millivolts, temp_c, temp_f);*/
}

process	main(void)
{
	recvclr();
	temp = create(tempread, 4096, 50, "tempread", 0);
	resched_cntl(DEFER_START);
	resume(temp);
	resched_cntl(DEFER_STOP);
	/*sleep(5);
	kprintf("LED Switched On");*/
	/*sensor_pin = "P9_40";
	kprintf("Starting reading sensor data \n");
	reading = read(ADC, sensor_pin, 5);
	kprintf("Read value: %d",reading);
    	millivolts = reading * 1800;  // 1.8V reference = 1800 mV
    	temp_c = (millivolts - 500) / 10;
    	temp_f = (temp_c * 9/5) + 32;
    	kprintf("mv=%d C=%d F=%d", millivolts, temp_c, temp_f);*/
    	sleep(5);
	return OK;
}
