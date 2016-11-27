from flask import Flask, jsonify
from flask import abort
import socket, pickle

app = Flask(__name__)
addrs = {}
devices = {}


def register():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Create a socket object
    host = "192.168.0.11"  # Get local machine name

    port = 7778  # Reserve a port for your service.
    s.bind((host, port))  # Bind to the port

    # global_dict = {}
    count = 2
    while count > 0:
        print("Listening ....")
        rec_msg, addr = s.recvfrom(1024)  # Establish connection with client.
        # pickle.dump(global_dict, open("save.p", "wb"))
        if "analog" in rec_msg :
            devices.__setitem__("analog", addr[0])
            addrs.__setitem__(addr[0], "analog")
        else :
            devices.__setitem__("digital", addr[0])
            addrs.__setitem__(addr[0], "digital")
        print 'registration successful.. for the addr : ', addr, ' recieve msg ...', rec_msg
        count -= 1
    return s

s = None

@app.route('/devices/', methods=['GET'])
def getDevices():
    # addrs = pickle.load(open("save.p", "rb"))
    return jsonify(dict(addrs))

@app.route('/device/<deviceid>', methods=['GET'])
def get_data(deviceid):
    # print(rec_msg)
    if deviceid is not None:
        if "analog" in deviceid :
            print 'sentto  : ', b"6060"
            s.sendto(b"6060", (devices.get("analog"), 7770))
            rec_msg, addr = s.recvfrom(1024)
            print 'Temperature sensor Data', rec_msg
            return rec_msg
        elif "digital" in deviceid :
            print 'led is switching on...'
            print 'sentto  : ', b"6061"
            s.sendto(b"6061", (devices.get("digital"),7770))
    elif deviceid is 'end':
        return "nothing"
        s.close()

if __name__ == '__main__':
    if s is None :
        s = register()
    app.run(debug=True, use_reloader=False)
