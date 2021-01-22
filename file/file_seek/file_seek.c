#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>  
#include <unistd.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <errno.h>


#define FILE_NAME	"biao_debug_file"

int main(int argc, char *argv[])
{
	int fd = 0;
	int l_ret = 0;

	fd = open(FILE_NAME, O_RDWR | O_CREAT | O_SYNC | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
	if(fd==0)
	{
		printf("open error \n");
	}

	//l_ret = lseek(fd, -102400, SEEK_END);
	//l_ret = lseek(fd, 102400, SEEK_END);
	l_ret = lseek(fd, -1000, SEEK_END);
	printf("ret = %d \n",l_ret);

	close(fd);

	return 0;


}
