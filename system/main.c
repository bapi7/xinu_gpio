/*  main.c  - main */

#include <xinu.h>
#include <string.h>
int producer_id;
uint32	ipaddr;
char	str[128];
int32	inlen,outlen;			/* Length of data received & data sent	*/

pid32 led,temp;

/*process ledon() {
    char buffer[10] = "";
    write(GPIO, buffer, 10);
    return OK;
}*/

uint32 getIP_int(char* ip){
	/* The return value. */ 
    uint32 remIP_int = 0;
    /* The count of the number of bytes processed. */
    int i;
    /* A pointer to the next digit to process. */
    char * start;

    start = ip;
    for (i = 0; i < 4; i++) {
        /* The digit being processed. */
        char c;
        /* The value of this byte. */
        int n = 0;
        while (1) {
            c = * start;
            start++;
            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - '0';
            }
            /* We insist on stopping at "." if we are still parsing
               the first, second, or third numbers. If we have reached
               the end of the numbers, we will allow any character. */
            else if ((i < 3 && c == '.') || i == 3) {
                break;
            }
            else {
                //INVALID IP;
				return SYSERR;
            }
        }
        if (n >= 256) {
            //INVALID IP;
			return SYSERR;
        }
        remIP_int *= 256;
        remIP_int += n;
    }
	return remIP_int;
}

uid32 firstCallToEdgeServer(){
	uid32 udpslotClient;
	kprintf("\n Entered producer .... \n");	
	net_init();
	
	ipaddr = getlocalip();
	if ((int32)ipaddr == SYSERR) {
		kprintf("Cannot obtain an IP address\n");
	} else {
		/* Print the IP in dotted decimal and hex */
		ipaddr = NetData.ipucast;
		sprintf(str, "%d.%d.%d.%d",
			(ipaddr>>24)&0xff, (ipaddr>>16)&0xff,
			(ipaddr>>8)&0xff,        ipaddr&0xff);
	
		kprintf("Obtained IP address  %s   (0x%08x)\n", str,
								ipaddr);
	}
	
	char * ip = "192.168.0.102";		//IP of Edge socket server
	
	uint32 remIp_int = getIP_int(ip);
    
	udpslotClient = udp_register (remIp_int,7778,7770);	//server IP, server/remote port, local port	192.168.0.7 = 3232235527
	if (udpslotClient == SYSERR) {
		kprintf("Cannot register with UDP\n");
		return SYSERR;
	}
	// udp_send(udpslotClient,str,strlen(str));
	int32 mode = 1; // mode 1 is analog
	udp_send(udpslotClient,"analog",strlen("analog"));
	if(mode == 1)
		adcinit(); 
	return udpslotClient;
}

int32 getMsgInt(char* msg){
	// kprintf("msg --> %s\n", msg);
	int32 n = 0;
	int32 i = 0 ;
	char c;
	for (i = 0; i < 4; i++) {
		c = (*msg) -  '0';
		n = (n*10) + c;
		// kprintf("msg --> %s %d %c\n", msg, n, c);
		msg++;
	}
	return n;
}


int32 tempread(int32 devid) {
	char * sensor_pin;
	int32 temp_c, temp_f, voltage_reading;
	sensor_pin = "P9_39";
	// int32 count = 20;
	// while(count > 0){
        kprintf("Starting reading sensor data \n---------------------------------\n");
        voltage_reading = pread(devid);
        kprintf("Voltage value read(in millivolts): %d\n",voltage_reading);
        temp_c = (voltage_reading - 500) / 10;
        temp_f = (temp_c * 9/5) + 32;
        kprintf("Temperature in Celsius = %d\nTemperature in Fahrenheit = %d\n", temp_c, temp_f);
		// count--;
	// }
	return temp_c;
}

void triggerled(){
	pwrite(LED, 1); // led on
	sleep(2);
	pwrite(LED,0);
	return;
}

process producer(void){
	
	// char*  msg;
	char*  replymsg;
	char* temp_value_str ;
	int32 replymsg_int ;
	temp_value_str = "6060"; 
	uid32 udpslotClient = firstCallToEdgeServer();
	replymsg = "-1";
	// replymsg = "No message from Server";
	
	kprintf("\n Msg sent to server.... \n");

	int32 count = 10 ;
	int32 temp_value; 
	while(count > 0 ){
		inlen = udp_recv(udpslotClient,replymsg,29,60000);
		if (inlen == TIMEOUT) {
			kprintf("\n Response from server Timed Out: 60000ms.... \n");
			continue;
		} else if (inlen == SYSERR) {
			kprintf("\n Response from server SYSERR.... \n");
			return SYSERR;
		}
		kprintf("\n Reply from server:: %d .... \n",replymsg);
		count--;
		replymsg_int = getMsgInt(replymsg);
		kprintf(" int value of msg %d\n", replymsg_int);
		switch(replymsg_int){
			case 6060:
				temp_value = tempread(TEMPSENSOR);
				sprintf(temp_value_str , "%d", temp_value);
				kprintf("Temp value read... : %d\n", temp_value);
				udp_send(udpslotClient,temp_value_str,strlen(temp_value_str));
				break;
			case 6061 : 
				triggerled();
				kprintf("led is flickering...\n");
				break;
		} 
	}

	
	udp_release(udpslotClient);
	return OK;
}

process	main(void)
{
	recvclr();
	kprintf("\n Entered Main.... \n");	
	producer_id  = create(producer, 4096, 50, "producer",0);
	
	resched_cntl(DEFER_START);	
	resume(producer_id);
	resched_cntl(DEFER_STOP);
	// temp = create(tempread, 4096, 50, "tempread", 0);
	// resched_cntl(DEFER_START);
	// resume(temp);
	// resched_cntl(DEFER_STOP);
	/*sleep(5);
	kprintf("LED Switched On");*/
	return OK;
}
