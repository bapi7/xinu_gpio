import socket               # Import socket module

s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)         # Create a socket object
host = "192.168.0.11" # Get local machine name

port = 7778                # Reserve a port for your service.
s.bind((host, port))       # Bind to the port

print("Listening ....")
rec_msg, addr = s.recvfrom(1024)     # Establish connection with client.
while True:
    # print(rec_msg)
    if rec_msg is not None:
        print 'sentto  : ', b"6060"
        s.sendto(b"6060", addr)
        rec_msg, addr = s.recvfrom(1024)
        print 'Temperature sensor Data', rec_msg
    elif rec_msg is 'end':
        s.close()
    # print('Got connection',addr)
    # s.sendto(b"Thank you for connecting",addr)
    

