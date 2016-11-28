pyCode folder consists of code for runing the edge server 
each beaglebone has the ip address of the edge server before connecting(static in the code of BBB).
First call from bbb to edge does the registration in the edge server. 

We are using Rest Api as interaction between Edge <--> Cloud.
	Api services provided are : 
		/devices  :  lists the ip address of the device and the type of the device
		/devices/analog : will trigger a call to the analog bbb to get the analog sensor data. This will be pictorially represented in a graphical format over the time. 
		/devices/digital : will flash the led when the rest call is made.
		/devices/stat : will give the graph over the time (default is 30sec triggers the analog reader for 2 sec consequtively) : partially done

DDL Generator : 
	Inputs : ddl.json : the device descriptor language framework we choosed is JSON. 
	generator.py : takes the ddl.json and generates pio.c which acsts as an itnerface to the I/O (specifically to read data from GPIO and Analog Pins)
	

Rest Server : 
	We are using Flask lib in python for implementing the REST. 
	app.py contains the code that routes the requests based on the routeurl. 
		
