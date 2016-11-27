import json, sys

dir = sys.argv[-2]
outputDir = sys.argv[-1]

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

def writeOut(out_file, out_list, arg, isData):
    data = ", data" if isData else ""
    returnVal = "return " if not isData else ""
    with open(out_file, "w+") as outf:
        for out in out_list :
            outf.write("case "+str(out["index"])+" : \n")
            outf.write("\t"+returnVal+ str(out["type"])+arg+"("+str(out["index"])+data+");\n\tbreak;\n")


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


def generateDDLfull(filename):
    with open(filename) as data_file:
        data = dict(json.load(data_file))
    out_list = []
    in_list = []
    devices = data["devices"]
    for device in devices :
        # pprint(device)
        for pin in device["pins"] :
            # print str(pin["mode"]), str(pin["mode"]) == "out" , str(pin["mode"]) == "in"
            if str(pin["mode"]) == "out" :
                out_list.append(pin)
            elif str(pin["mode"]) == "in" :
                in_list.append(pin)
            else :
                print "Unknown mode found...Correct the DDL"

    # print "outL : ", out_list
    # print "inL : ", in_list
    # pprint(dict(data))
    in_file = dir+"ddl_io_in.inc"
    out_file = dir+"ddl_io_out.inc"

    writeOut(out_file, out_list, "_write",True)
    writeOut(in_file, in_list, "_read", False)

    return (in_file, out_file)




base_filename = generateIO()
input_output_list = generateDDLfull(dir+ "ddl.json")
merge(base_filename, input_output_list[0], input_output_list[1], "ddl_io_in.inc", "ddl_io_out.inc")

