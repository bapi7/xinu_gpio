from flask import Flask, jsonify
from flask import abort
from json2html import *
import socket, pickle


import pygal
from flask import render_template
from datetime import datetime
from pygal.style import TurquoiseStyle

app = Flask(__name__)
analog_addrs = {}
digital_addrs = {}
addrs = []
devices = {}


def renderGraph(rec_msg):
    FORMAT = '%H:%M'
    file = open("list.txt", "a")
    file.write(str(rec_msg) + " " + str(datetime.now().strftime(FORMAT)) + "\n")
    file.close()

    x, y = [], []
    with open('list.txt') as f:
        for l in f:
            row = l.split()
            x.append(row[0])
            y.append(row[1])
    t = [int(numeric_string) for numeric_string in x]
    try:
        graph = pygal.StackedLine(fill=True, interpolate='cubic', style=TurquoiseStyle)
        graph.title = 'Temperature in Celsius'
        graph.x_labels = y

        graph.add('temperature', t)
        graph_data = graph.render_data_uri()
        return graph.render_response()
    except Exception, e:
        return (str(e))


def register():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Create a socket object
    host = "192.168.0.102"  # Get local machine name

    port = 7778  # Reserve a port for your service.
    s.bind((host, port))  # Bind to the port

    # global_dict = {}
    count = 1
    while count > 0:
        print("Listening ....")
        rec_msg, addr = s.recvfrom(1024)  # Establish connection with client.
        # pickle.dump(global_dict, open("save.p", "wb"))
        if "analog" in rec_msg :
            devices.__setitem__("analog", addr[0])
            analog_addrs.__setitem__("Device", addr[0])
            analog_addrs.__setitem__("Sensor", "Temperature Sensor")
            analog_addrs.__setitem__("Sensor Type", "analog")
            addrs.append(analog_addrs)
        else :
            devices.__setitem__("digital", addr[0])
            digital_addrs.__setitem__("Device", addr[0])
            digital_addrs.__setitem__("Sensor", "LED")
            digital_addrs.__setitem__("Sensor Type", "digital")
            addrs.append(digital_addrs)
        print 'registration successful.. for the addr : ', addr, ' recieve msg ...', rec_msg
        count -= 1
    return s

s = None

@app.route('/devices/', methods=['GET'])
def getDevices():
    return render_template("test.html",devices = addrs)

@app.route('/sensorstatus/<deviceid>/', methods=['GET'])
def get_data(deviceid):
    # print(rec_msg)
    if deviceid is not None:
        if "analog" in deviceid :
            print 'sentto  : ', b"6060"
            s.sendto(b"6060", (devices.get("analog"), 7770))
            rec_msg, addr = s.recvfrom(1024)
            print 'Temperature sensor Data', rec_msg
            return renderGraph(rec_msg)
            # return rec_msg

        elif "digital" in deviceid :
            print 'led is switching on...'
            print 'sentto  : ', b"6061"
            s.sendto(b"6061", (devices.get("digital"),7770))
            return 'Checkout LED :)'
    elif deviceid is 'end':
        return "nothing"
        s.close()

if __name__ == '__main__':
    if s is None :
        s = register()
    app.run(debug=True, use_reloader=False)
