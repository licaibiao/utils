#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<fcntl.h> 
#include<limits.h> 
#include<string.h>

#define BUFSZ PIPE_BUF 

#define PROCESS_NAME	"main"


int check_status(char * name)
{
	FILE* fp; 
	char * line = NULL;
	size_t len = 0;
	char command[150]; 
	int l_s32Ret = 0;
	ssize_t read;
	
	if(NULL==name)
	{
		printf("input para is null \n");
		return -1;
	}
	
	sprintf(command, "ps | grep %s", name); 
	
	if((fp = popen(command,"r")) == NULL) 
	{
		printf("popen error \n");
		return -2;
	}
	
	while ((read = getline(&line, &len, fp)) != -1) 
	{
		if(NULL!=strstr(line,"/stone/main"))
		{
			printf("\n %s is alive \n",PROCESS_NAME);
			l_s32Ret = 1;
		}
    }

	free(line);
	pclose(fp); 
	
	return l_s32Ret;	
}

int get_system_log(void)
{
	system("dmesg > /config/dmesg.log");
	system("logcat -d > /config/logcat.log");
	sync();
}



int main(int argc, char *argv[]) 
{
	int l_s32Status = 0;
	while(1)
	{
		l_s32Status = check_status(PROCESS_NAME);
		
		if(l_s32Status>0) //正常运行
		{
			
		}else if(l_s32Status==0) //程序挂了
		{
			get_system_log();
			exit(0); //退出该进程
			
		}else //参数不对
		{
						
		}
		
		sleep(2);
	}

	
	return 0;
} 
