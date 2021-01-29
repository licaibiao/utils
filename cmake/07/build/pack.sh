#!/bin/bash

ROOT_DIR=`pwd`                                                                                                                                    
BIN_DIR=${ROOT_DIR}/pack/bin
if [ ! -d ${BIN_DIR} ]; then
	mkdir ${BIN_DIR}
fi



OPTION=$(whiptail --title "select your board type" --menu   \
    "What is the your build type?" 15 60 4    \
    "main" ""              \
    "sub" ""            \
    3>&1 1>&2 2>&3)

exitstatus=$?
if [ $exitstatus = 0 ]; then
    echo "the chosen board type is:" $OPTION
else
	echo "you chose cancel."
fi

if [ $OPTION == "main" ]; then
	echo "main"
else
	echo "sub"	
fi	


if [ $exitstatus = 0 ]; then
	echo "end"
fi


