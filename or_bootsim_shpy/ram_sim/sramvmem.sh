#!/bin/bash
##

if [ "$1" = "" ]   # || [ "$2" ="" ]
then
    echo "please enter an *.S/s file name"
    exit 1
fi

or32-elf-as $1 -o $1.o
or32-elf-objdump -d $1.o > $1.txt

python hyexpram.py   
##must edit py filename use $1

CPPATH=/mnt/share/
cp $1.txt $CPPATH
##cp bootrom.v $CPPATH
##cp bootrom.coe $CPPATH
cp sram.vmem $CPPATH

rm $1.o
less $1.txt
