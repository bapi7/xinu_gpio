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
	int32 temp_c, temp_f, voltage_reading;
	sensor_pin = "P9_39";
        kprintf("Starting reading sensor data \n");
        voltage_reading = read(ADC, sensor_pin, 10);
        kprintf("Voltage value read(in millivolts): %d\n",voltage_reading);
        temp_c = (voltage_reading - 500) / 10;
        temp_f = (temp_c * 9/5) + 32;
        kprintf("Temperature in Celsius = %d\nTemperature in Fahrenheit = %d\n", temp_c, temp_f);
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
	return OK;
}
