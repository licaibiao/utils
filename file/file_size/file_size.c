#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define FILE_NAME	"biao_debug_file"


int file_size2(char* filename) 
{ 
  struct stat statbuf; 
  stat(filename,&statbuf); 
  int size=statbuf.st_size; 
 
  return size; 
} 




int main(int argc, char *argv[])
{
	FILE* l_pF = NULL;
	int l_s32Size = 0;

	l_pF = fopen(FILE_NAME,"r");
	if(l_pF==NULL)
	{
		printf("open error\n");
	}else
	{
		

		l_s32Size = file_size2(FILE_NAME);
		
		printf("%s open ok,file size = %d\n",FILE_NAME,l_s32Size);
		
	}

	return 0;

}
