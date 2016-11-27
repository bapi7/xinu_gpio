import socket, pickle
s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)         # Create a socket object
host = "192.168.0.11" # Get local machine name

port = 7778                # Reserve a port for your service.
s.bind((host, port))       # Bind to the port

global_dict = {}
count = 2
while count > 0 :
    print("Listening ....")
    rec_msg, addr = s.recvfrom(1024)     # Establish connection with client.
    global_dict.__setitem__(addr, rec_msg)
    pickle.dump(global_dict, open("save.p", "wb"))
    print 'registration successful.. for the addr : ', addr, ' recieve msg ...', rec_msg
    count -= 1


# while True:
#     # print(rec_msg)
#     if rec_msg is not None:
#         print 'sentto  : ', b"6061"
#         s.sendto(b"6061", addr)
#         rec_msg, addr = s.recvfrom(1024)
#         print 'Temperature sensor Data', rec_msg
#     elif rec_msg is 'end':
#         s.close()


    # print('Got connection',addr)
    # s.sendto(b"Thank you for connecting",addr)

