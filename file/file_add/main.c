
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
 
#define BOOT_FILE_NAME		"./hc32l110_boot.bin"
#define LEN					(4*1024)

int main(void)
{
	FILE *l_pf = NULL;
	struct stat st;
	int l_s32Ret = 0;
	int size = 0;
	unsigned char l_data = 0xFF;
	int i = 0;
 
	stat(BOOT_FILE_NAME, &st);
	size = st.st_size;
 
	l_pf = fopen(BOOT_FILE_NAME,"r+");
	if(NULL==l_pf)
	{
		printf("line %d error \n",__LINE__);
		return -1;
	}
	
	fseek(l_pf, 0, SEEK_END);
	for(i=0;i<(LEN-size);i++)
	{
		fwrite(&l_data,1,1,l_pf);
	}
	
	fclose(l_pf);

	return 0;

}
