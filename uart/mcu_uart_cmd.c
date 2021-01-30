
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int data_genarate(unsigned char *pData,char len)
{
	unsigned char l_arru8Data[32] = {0};
	unsigned char xor_val = 0;
	int i = 0;

	l_arru8Data[0] = 0x7b;
	l_arru8Data[1] = len + 4;
	l_arru8Data[2] = 0x90;
	memcpy(&l_arru8Data[3],pData ,len);

	for (i = 0; i < len + 3; i++)
	{
		xor_val ^= l_arru8Data[i];
	}

	l_arru8Data[len+3] = xor_val;

	for(i=0;i<len+4;i++)
	{
		printf("%02x ",l_arru8Data[i]);
	}
	printf("\n");

}


int test1(void)
{
	unsigned short time = 0;
	unsigned char data[16] = {2,2,1,3,2,0x14,0xfa,0,0x74,0x04};

	while(time!=0xffff)
	{
		data[9] = time;
		data[8] = time >>8;
		data_genarate(data,10);

		time++;
	}

	
}

int main(void)
{

	unsigned short time = 0;
	unsigned char data[16] = {2,0x7b,1,3,2,0x14,0x7b,0,0x7b,0x04};

	data_genarate(data,10);

}
