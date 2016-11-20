/*  main.c  - main */

#include <xinu.h>

pid32 led;

process ledon() {
    char buffer[10] = "";
    write(GPIO, buffer, 10);
    return OK;
}

process	main(void)
{
	recvclr();
	led = create(ledon, 4096, 50, "ledon", 0);
	resched_cntl(DEFER_START);
	resume(led);
	resched_cntl(DEFER_STOP);
	sleep(5);
	kprintf("LED Switched On");
	return OK;
}
