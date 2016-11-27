#!flask/bin/python
from flask import Flask, jsonify
from flask import abort
import socket

app = Flask(__name__)

def register():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Create a socket object
    host = "192.168.0.11"  # Get local machine name

    port = 7778  # Reserve a port for your service.
    s.bind((host, port))  # Bind to the port

    print("Listening ....")
    rec_msg, addr = s.recvfrom(1024)  # Establish connection with client.
    return addr

@app.route('/status', methods=['GET'])
def get_data():
    while True:
        # print(rec_msg)
        if rec_msg is not None:
            print 'sentto  : ', b"6060"
            s.sendto(b"6060", addr)
            rec_msg, addr = s.recvfrom(1024)
            print 'Temperature sensor Data', rec_msg
            return rec_msg
        elif rec_msg is 'end':
            return "nothing"
            s.close()


if __name__ == '__main__':
    app.run(debug=True)
