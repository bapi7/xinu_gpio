from flask import Flask, jsonify
from flask import abort
from json2html import *
import socket, pickle, random
import time

import pygal
from flask import render_template
from flask_restful import Resource, Api

from datetime import datetime
from pygal.style import TurquoiseStyle

app = Flask(__name__)
analog_addrs = {}
digital_addrs = {}
addrs = []
devices = {}

def renderUI(t,y):
    t = [int(numeric_string) for numeric_string in t]
    try:
        graph = pygal.StackedLine(fill=True, interpolate='cubic', style=TurquoiseStyle)
        graph.title = 'Temperature in Celsius'
        graph.x_labels = y

        graph.add('temperature', t)
        graph_data = graph.render_data_uri()
        return graph.render_response()
    except Exception, e:
        return (str(e))

def renderGraph(rec_msg):
    FORMAT = '%H:%M'
    file = open("list.txt", "a")
    file.write(str(rec_msg) + " " + str(datetime.now().strftime(FORMAT)) + "\n")
    file.close()

    x, y = [], []
    with open('list.txt') as f:
        for l in f:
            rowx, rowy = l.split()
            x.append(rowx)
            y.append(rowy)
    t = [int(numeric_string) for numeric_string in x]
    return renderUI(t,y)


def register():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Create a socket object
    host = "192.168.0.100"  # Get local machine name

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
            analog_addrs.__setitem__("device", addr[0])
            analog_addrs.__setitem__("sensor", "Temperature Sensor")
            analog_addrs.__setitem__("sensor_type", "analog")
            addrs.append(analog_addrs)
        else :
            devices.__setitem__("digital", addr[0])
            digital_addrs.__setitem__("device", addr[0])
            digital_addrs.__setitem__("sensor", "LED")
            digital_addrs.__setitem__("sensor_type", "digital")
            addrs.append(digital_addrs)
        print 'registration successful.. for the addr : ', addr, ' recieve msg ...', rec_msg
        count -= 1
    return s

s = None

@app.route('/devices/', methods=['GET'])
def getDevices():
    return render_template("test.html",devices = addrs)

@app.route('/devices/interval/<timee>', methods=['GET'])
def getDataInInterval(timee) :
    count = int(timee)
    arr = []
    while(count > 0 ):
        s.sendto(b"6060", (devices.get("analog"), 7770))
        rec_msg, addr = s.recvfrom(1024)
        # rec_msg = random.randint(50,100)
        arr.append(str(rec_msg)+ " " + str(datetime.now().strftime("%H:%M:%S")))
        time.sleep(1)
        print arr, count
        count -= 1
    arrs = [i.split() for i in arr]
    t = [int(i[0].strip()) for i in arrs]
    y = [i[1].strip() for i in arrs]
    return renderUI(t, y)

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
        elif "interval" in deviceid :
            print 'interval found'
    elif deviceid is 'end':
        return "nothing"
        s.close()


if __name__ == '__main__':
    if s is None :
        s = register()
    app.run(debug=True, use_reloader=False)
