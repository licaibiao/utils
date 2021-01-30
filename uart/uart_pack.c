#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UART_BUF	28
static unsigned char g_u8RXCount = 0;
static unsigned char gs_arru8UartData[MAX_UART_BUF] = {0};


int data_unpack(unsigned char Data)
{
	int i = 0;
	int l_u8DataLen = 0;
	unsigned char l_u8XorValue = 0;
	
    gs_arru8UartData[g_u8RXCount++] = Data;

	if((g_u8RXCount==3)&&(gs_arru8UartData[0]!=0x5a)&&((gs_arru8UartData[1]!=0x5a)))
	{
		for(l_u8DataLen = 0; l_u8DataLen < MAX_UART_BUF; l_u8DataLen++)
		{
			gs_arru8UartData[l_u8DataLen] = 0;
		}
		g_u8RXCount = 0;
	}else if(g_u8RXCount>=MAX_UART_BUF)
	{
		for(l_u8DataLen = 0; l_u8DataLen < MAX_UART_BUF; l_u8DataLen++)
		{
			gs_arru8UartData[l_u8DataLen] = 0;
		}
		g_u8RXCount = 0;
	}
    else
    {
        if((g_u8RXCount > 4) && (g_u8RXCount == gs_arru8UartData[2]))
        {
			//校验数据
			for(l_u8DataLen = 0; l_u8DataLen < g_u8RXCount - 3; l_u8DataLen++)
			{
				l_u8XorValue ^= gs_arru8UartData[l_u8DataLen];	
			}
			
			if((gs_arru8UartData[0]==0x5a)&&(gs_arru8UartData[0]==0x5a)&&((gs_arru8UartData[g_u8RXCount-1]==0xa5))
				&&(gs_arru8UartData[g_u8RXCount-2]==0xa5)&&(gs_arru8UartData[g_u8RXCount - 3] == l_u8XorValue))
		    {
				printf("data ok \n");
				
			}else 
			{
				//printf("data error \n");			
			}
	
            for(l_u8DataLen = 0; l_u8DataLen < MAX_UART_BUF; l_u8DataLen++)
			{
				gs_arru8UartData[l_u8DataLen] = 0;
			}
            
            g_u8RXCount = 0;
            l_u8XorValue = 0;
        }
    }
}

int data_pack(unsigned char *pData,char len)
{
	unsigned char l_arru8Data[MAX_UART_BUF] = {0};
	unsigned char xor_val = 0;
	int i = 0;
	int j = 0;

	l_arru8Data[0] = 0x5a;
	l_arru8Data[1] = 0x5a;
	l_arru8Data[2] = len + 7;
	l_arru8Data[3] = 0x90;
	memcpy(&l_arru8Data[4],pData ,len);

	for (i = 0; i < len + 4; i++)
	{
		xor_val ^= l_arru8Data[i];
	}

	l_arru8Data[len+4] = xor_val;
	l_arru8Data[len+5] = 0xa5;
	l_arru8Data[len+6] = 0xa5;

	for(i=0;i<len+7;i++)
	{
		printf("%02x ",l_arru8Data[i]);
			
	}
	printf("\n");
#if 0	
	for(i=0;i<len+7;i++)
	{
		data_unpack(l_arru8Data[i]);
			
	}
#else 
	for(j=0;j<10;j++)
	{
		for(i=0;i<len+7;i++)
		{
			if((0==j)&&(i==4))
			{
				//data_unpack(0x5a);
			}else 
			{
			    data_unpack(l_arru8Data[i]);
				
			}

		}		
	}	
#endif	
}


int main(void)
{

	unsigned short time = 0;
	unsigned char data[16] = {2,0x5a,1,0x5a,2,0x14,0x7b,0,0x7b,0x04};

	data_pack(data,10);

#if 0	
	while(time!=0xffff)
	{
		data[9] = time;
		data[8] = time >>8;
		data_pack(data,10);

		time++;
	}
#endif	
	

}

