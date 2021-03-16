#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>



























#define DEV_PATH                "./emmc/devcheck.ini"

int park_write_emmc(void)
{
    int l_s32len = -1;
    FILE *l_Fp = NULL;
    char * l_s8Data = "Hello";

    l_Fp = fopen(DEV_PATH,"wb+");
    if(NULL!=l_Fp)
    {
        l_s32len = fwrite(l_s8Data, 1, strlen(l_s8Data), l_Fp);  
        fclose(l_Fp);

        if(l_s32len>0)
        {
            printf("%s emmc write ok \n",__FUNCTION__);
            remove(DEV_PATH);
            return 1;
        } 
    }else
    {
        printf("%s emmc open error \n",__FUNCTION__);
    }

    printf("%s emmc write error %d \n",__FUNCTION__,l_s32len);
     
    return 0;
}















#define HERROR_CLOSE(exp, err)  do {                                                    \
    if (exp) {                                              \
        printf("%s\n", #exp);                           \
    }                                                       \
} while(0);



























 main(void)
{
	FILE * stream = NULL;
	char i = 0;
	//park_write_emmc();
	HERROR_CLOSE(NULL == stream, 0x20);
	return 0;

}
