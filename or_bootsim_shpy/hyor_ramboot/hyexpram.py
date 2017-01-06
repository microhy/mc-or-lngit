# -*- coding: utf-8 -*-
#!/bin/bash
"""
Created on Thu Dec 29 11:36:21 2016

@author: Microhy
"""
import sys
import re

filename = sys.argv[1]
fd = file(filename)
lines = fd.readlines()
fd.close()

count = 0
hexcode = []
bootrom = []
for line in lines:
    match = re.search("([0-f]{2} ){4}", line)
    if match:
    	result = match.group()
        temp = result.split()
        hexcodetmp = temp[0]+temp[1]+temp[2]+temp[3]
        bootromtmp = str(count)+": wb_dat_o <= 32'h" + hexcodetmp + ";\n"
        hexcodetmp = hexcodetmp + '\n'
        hexcode.append(hexcodetmp)
        bootrom.append(bootromtmp)
        count = count+1
    else:
    	result = ""

#filename = "bootrom.v"
#fd = file(filename,'w')
#fd.writelines(bootrom)
#fd.close()

hexcode[len(hexcode)-1] = hexcode[len(hexcode)-1].replace(',',';')
filename = "sram.vmem" 
fd = file(filename,'w')
temp = ['cccccccc\n' for n in range(int('0x100',16)/4)]
fd.writelines(temp)
fd.writelines(hexcode)
fd.close()

