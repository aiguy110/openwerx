#!/usr/bin/python3
import os, sys

# Get partion number from command line args
if len(sys.argv) > 2:
    print("Wrong number of arguments. Aborting...")
    exit()
part_num = sys.argv[1]
if part_num not in ['2', '3', '4']:
    print("Invalid partion number. Aborting...")
    exit()

# Read in the contents of cmdline.txt
f = open("/boot/cmdline.txt", "r")
old_cmd = f.read()
f.close()

# Find key points in the command
# ... root=PARTUUID=dd3bb636-02 ...
#          0123456789         ^
i0=old_cmd.find('PARTUUID=')
i1=old_cmd.find(' ', i0)

# Generate new command
old_uuid = old_cmd[i0+9: i1]
new_uuid = old_uuid[:-1] + part_num 
new_cmd = old_cmd[:i0+9] + new_uuid + old_cmd[i1:]

# Write the new command to the file
f = open("/boot/cmdline.txt", "w")
f.write(new_cmd)
f.close()
