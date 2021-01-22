

#include<stdio.h>
#include<stdlib.h>

int func_hello(void)
{
	printf("%s %s I am hello\n",__FILE__,__FUNCTION__);
	return 0;
}
