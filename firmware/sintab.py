#!/usr/bin/python
#
import sys
import math

interval=(3.141592 * 2) / 3

print("unsigned char sintab[] = {")
for x in range(0,256):
    val = int(math.floor(math.sin((interval*x)/256)*256))
    if (x % 8 == 0):
        sys.stdout.write("        ")
    
    sys.stdout.write("0x%02x," % val)

    if ((x % 8) == 7):
        sys.stdout.write("\n")
    else:
        sys.stdout.write(" ")
print("};")
