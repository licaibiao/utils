#!/bin/sh 

rm -rf `ls|grep -v "\.sh\|pack_sdk"`

#cmake .. -DCROSS_COMPILE=mips-linux-uclibc-gnu-\
cmake .. -DCROSS_COMPILE=\
	-DBOARD=main\
	-DVERSION=debug\
	-DWIFI_SUPPORT=OFF\
	-DEMMC_SUPPORT=ON


make


