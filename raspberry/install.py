#!/usr/bin/python3
import os, sys

# Configure module to run at boot
if len(sys.argv) != 2:
    print("Incorrect number of arguments.")
    print("USAGE: ")
    print("> sudo ./install <active_module>")
    print("Where <active_module> is the module whose configuration")
    print("the software should assume after being lauched at boot-time.")  
    exit()

os.system('echo "{}" > ./active_module'.format(sys.argv[1]))

# Install new rc.local file
inFile = open('data/rc.local', 'r')
outFile = open('/etc/rc.local', 'w')
inStr = inFile.read()
outStr=inStr.format(os.getcwd())
outFile.write(outStr)
outFile.close()
inFile.close()
