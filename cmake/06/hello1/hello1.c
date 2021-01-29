
#include <stdio.h>
#include <stdlib.h>


void func_hello1(void)
{
	printf("%s \n",__FUNCTION__);
	
#ifdef EMMC_SUPPORT
	printf("have define emmc \n");

#else
	printf("no define emmc \n");


#endif


}
