#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int make_error(void)
{
	char l_buf[10240000];
	int i = 0;
	char *p = NULL;
	while(i++)
	{
		l_buf[i++] = i;
		l_buf[i++] = i;
		l_buf[i++] = i;
		l_buf[i++] = i;
		l_buf[i++] = i;
		l_buf[i++] = i;
		l_buf[i++] = i;
		memcpy(p,l_buf,1024000);
	}
}


int main(void)
{
	sleep(1);
	while(1)
	{
		//make_error();
		sleep(1);
	}
	
}
