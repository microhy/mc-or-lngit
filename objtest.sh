#!/bin/bash
##


if [ "$1" = "" ]   # || [ "$2" ="" ]
then
    echo "please enter file name"
    exit 1
fi

or32-elf-as $1 -o $1.o
or32-elf-objdump -d $1.o > $1.txt

CPPATH=/mnt/share/
cp $1.txt $CPPATH
rm $1.o

less $1.txt

