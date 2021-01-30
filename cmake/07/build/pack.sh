#!/bin/bash




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


echo Please input your version
 	read VERSION

ROOT_DIR=`pwd`                                                                                                                                    
BIN_DIR=${ROOT_DIR}/install/${OPTION}_${VERSION}/bin
if [ ! -d ${BIN_DIR} ]; then
	mkdir -p ${BIN_DIR}
fi

LIB_DIR=${ROOT_DIR}/install/${OPTION}_${VERSION}/lib
if [ ! -d ${LIB_DIR} ]; then
	mkdir -p ${LIB_DIR}
fi

	cp lib/* $LIB_DIR -rf
	cp bin/* $BIN_DIR -rf


#if [ $OPTION == "main" ]; then
#	echo "chose main"
#else
#	echo "chsoe sub"	
#fi	




