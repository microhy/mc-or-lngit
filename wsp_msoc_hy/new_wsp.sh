#!/bin/bash

if [ "$1" = "" ]
then
	echo "<err> Please enter workspace name"
	echo "<wrr> Do not have space for name"
	exit 1
fi

WSPNAME=$1
rm -rf $WSPNAME

PRJ_HOST=$WSPNAME/prj_host
mkdir -p $PRJ_HOST 

touch $PRJ_HOST/main_host.c
touch $PRJ_HOST/Makefile
echo "<ok> touch main_host.c ok!"


touch $WSPNAME/Makefile
touch $WSPNAME/mkconfig
mkdir $WSPNAME/com_lib_cu
touch $WSPNAME/com_lib_cu/ram.ld
touch $WSPNAME/com_lib_cu/reset.s

CUNUM=16
CUCNT=0
while [ $CUCNT -lt $CUNUM ]
do
	if [ $CUCNT -lt 10 ]
	then
		CUNAME=cu0$CUCNT
	else
		CUNAME=cu$CUCNT
	fi
	PRJ_CU=$WSPNAME/prj_$CUNAME
	MAIN_CU=main_$CUNAME.c
	mkdir $PRJ_CU 
	touch $PRJ_CU/$MAIN_CU
	touch $PRJ_CU/Makefile
	CUCNT=`expr $CUCNT + 1`
done

CUCNT=`expr $CUCNT - 1`
echo "<ok> mkdir prj_cu0~$CUCNT ok!" 

cd $1
echo "<workspace pwd> = $PWD"
ls -l

