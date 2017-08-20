#!/usr/bin/python3
import os, sys

if len(sys.argv) != 2:
    print("Usage: ./save <save-name>")
    exit()

if os.path.exists("./utils/backups/"+sys.argv[1]):
    os.system("sudo rm -rf ./utils/backups/"+sys.argv[1])

filePaths = [line for line in open('./utils/backup.list')]

for folderPath, fileName in map(os.path.split, filePaths):
    os.system("mkdir -p "+os.path.join("./utils/backups/", sys.argv[1], folderPath) )
    os.system("sudo cp -r "+os.path.join(folderPath, fileName)+" "+os.path.join("./utils/backups/",sys.argv[1], folderPath,fileName) )
