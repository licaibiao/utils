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

int pipe_fd = -1;

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
	system("date >> /config/dmesg.log");
	system("logcat -d > /config/logcat.log");
	system("date >> /config/logcat.log");
	sync();
}

int set_heartbeat_log(void)
{
	system("echo \"heartbeat no recv \" >> /config/heartbeat.log");
	system("date >> /config/heartbeat.log");
	sync();
}

int init_pipe(void)
{
	const char *fifo_name = "/tmp/my_fifo";
	int res = 0;
	int open_mode = O_RDONLY|O_NONBLOCK;

	printf("daemon opening FIFO O_RDONLY\n");
	pipe_fd = open(fifo_name, open_mode);
	if(pipe_fd<0)
	{
		printf("daemon open pipe error \n");
		return 0;
	}else
	{
		return -1;
	}	
}

int read_pipe_date(void)
{
	int res = 0;
	char buffer[PIPE_BUF + 1];
	memset(buffer, '\0', sizeof(buffer));
	res = read(pipe_fd, buffer, PIPE_BUF);
	if(res>0)
	{
		printf("read data from name pipe :%s",buffer);
		return 0;
	}else
	{
		printf("read data error \n");
		return -1;
	}
}

int exit_pipe(void)
{
	if(pipe_fd>0)
	{
		close(pipe_fd);
		pipe_fd = -1;
	}
}

int main(int argc, char *argv[]) 
{
	int l_s32Status = 0;
	int l_s32PipeStatus = 0;
	int l_s32ErrorCnt = 0;
	
	sleep(3);
	init_pipe();
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

		l_s32PipeStatus = read_pipe_date();
		if(l_s32PipeStatus!=0)
		{
			l_s32ErrorCnt++;
			printf("l_s32ErrorCnt %d \n",l_s32ErrorCnt);
			
		}else
		{
			l_s32ErrorCnt = 0;
		}
		
		if(l_s32ErrorCnt>10)
		{
			set_heartbeat_log();
			break;
			
		}
		
		sleep(1);
	}

	exit_pipe();

	return 0;
} 
