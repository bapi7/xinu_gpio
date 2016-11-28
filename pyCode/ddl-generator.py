import json, sys

dir = sys.argv[-3]
outputDir = sys.argv[-2]
includeDir = sys.argv[-1]

from pprint import pprint

arr = [
    "#include <xinu.h>\n"
    "int32 pread(int32 dev_id){",
    "switch (dev_id)",
    "{",
    "#include <ddl_io_in.inc>",
    "default:",
     "return -1;",
    "}",
    "}",
    "int32 pwrite(int32 dev_id, int32 data){",
    "switch (dev_id)",
    "{",
    "#include <ddl_io_out.inc>",
    "default:",
    "break;",
    "}",
    "return OK; "
    "}"]

def getPinNo(jobj, filename):
    with open(filename) as data_file:
        data = dict(json.load(data_file))
    if jobj["type"] == "gpio":
        id = jobj["id"]
        pinno = data["header"][id.split("_")[0]]["PINS"][id]["pinno"]
        return pinno
    elif jobj["type"] == "adc":
        return data["AnalogInputs"][jobj["id"]]

def writeOut(out_file, out_list, arg, isData, filename):
    data = ", data" if isData else ""
    returnVal = "return " if not isData else ""
    with open(out_file, "w+") as outf:
        for out in out_list :
            outf.write("case "+str(out["device"])+" : \n")
            outf.write("\t"+returnVal+ str(out["type"])+arg+"("+str(getPinNo(out,filename))+data+");\n\tbreak;\n")


def generateIO():
    tabC = 0
    prevLine = None
    filename = outputDir+"pio.c"
    with open(filename, "w") as f :
        for line in arr :
            if line.endswith("{"):
                f.write(''.join(['\t']*tabC) + line+"\n")
                tabC += 1
            elif line.startswith("}"):
                tabC -= 1
                f.write(''.join(['\t'] * tabC) + line + "\n")
            else :
                if prevLine.endswith(":"):
                    f.write(''.join(['\t'] * (tabC+1)) + line + "\n")
                else:
                    f.write(''.join(['\t'] * tabC) + line + "\n")
            prevLine = line
    f.close()
    return filename


def merge(base, inn, out, infilestr, outfilestr):
    final_lines = []
    with open(base, "r") as f :
        lines = f.readlines()
    for line in lines :
        if infilestr in line:
            tc = line.count("\t")
            with open(inn, "r") as innfile  :
                inlines = innfile.readlines()
                innfile.close()
            final_lines.extend([''.join(["\t"] * tc)+i for i in inlines])
            continue
        if outfilestr in line:
            tc = line.count("\t")
            with open(out, "r") as innfile:
                inlines = innfile.readlines()
                innfile.close()
            final_lines.extend([''.join(["\t"] * tc)+i for i in inlines])
            continue

        final_lines.append(line)

    with open(base, "w+") as f :
        f.writelines(final_lines)
        f.close()

def generateDevices(filename, ddict):
    with open(filename, 'wb+')as f :
        for key in ddict :
            f.write("#define "+key+" "+str(ddict.__getitem__(key))+"\n")
    f.close()


def generateDDLfull(filename):
    with open(filename) as data_file:
        data = dict(json.load(data_file))
    out_list = []
    in_list = []
    devices = data["devices"]
    devices_dict = {}
    count = 0
    for device in devices :
        devices_dict.__setitem__(str(device["name"]), count)

        # pprint(device)
        for pin in device["pins"] :
            # print str(pin["mode"]), str(pin["mode"]) == "out" , str(pin["mode"]) == "in"
            if str(pin["type"]) == "gpio":
                generateGPIOH(dir + "ddl.json", pin["id"])
            if str(pin["mode"]) == "out" :
                pin["device"] = count
                out_list.append(pin)
            elif str(pin["mode"]) == "in" :
                pin["device"] = count
                in_list.append(pin)
            else :
                print "Unknown mode found...Correct the DDL"
        count += 1

    generateDevices(includeDir +"devices.h", devices_dict)
    # print "outL : ", out_list
    # print "inL : ", in_list
    # pprint(dict(data))
    in_file = dir+"ddl_io_in.inc"
    out_file = dir+"ddl_io_out.inc"

    writeOut(out_file, out_list, "_write",True, filename )
    writeOut(in_file, in_list, "_read", False, filename)
    return (in_file, out_file)


def generateGPIOH(filename, id):
    with open(filename) as data_file:
        data = dict(json.load(data_file))
    pin = data["header"][id.split("_")[0]]["PINS"][id]
    base = pin["base"]
    pinno = pin["pinno"]

    headerObj = data["baseheader"][base]
    lines = []
    for key in sorted(headerObj) :
        lines.append(str("#define "+key + " "+headerObj.get(key))+"\n")
    # print lines

    with open(includeDir +"gpio.h", "wb+") as f :
        f.writelines(lines)
    f.close()


base_filename = generateIO()
input_output_list = generateDDLfull(dir+ "ddl.json")
merge(base_filename, input_output_list[0], input_output_list[1], "ddl_io_in.inc", "ddl_io_out.inc")

