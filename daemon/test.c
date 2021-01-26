#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h> 


#include <signal.h>

int pipe_fd = -1;

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

void pipe_out(int sig)
{
	//do nothing
}

int init_pipe(void)
{
    const char *fifo_name = "/tmp/my_fifo";
    int data_fd = -1;
    int res = 0;
    const int open_mode = O_WRONLY|O_NONBLOCK;

	signal(SIGPIPE,pipe_out);

    if(access(fifo_name, F_OK) == -1)
    {
        printf ("Create the fifo pipe.\n");
        res = mkfifo(fifo_name, 0777);

        if(res != 0)
        {
            fprintf(stderr, "Could not create fifo %s\n", fifo_name);
            return -1;
        }
    }

    printf("Process %d opening FIFO O_WRONLY\n", getpid());
    pipe_fd = open(fifo_name, open_mode);
    printf("Process %d result %d\n", getpid(), pipe_fd);

    if(pipe_fd ==-1)
    {
		return -2;
	}

	return 0;

}

void send_pipe_date(void)
{
	int res = 0;
	char test_data[PIPE_BUF];
	static int i = 0;
	sprintf(test_data,"%d \n",i++);
	printf("Process %d write data to name pipe: %s",getpid(),test_data);
	
	res = write(pipe_fd, test_data, PIPE_BUF);
	if(res == -1)
	{
		printf("write error on name pipe !\n");
	}
	else
	{

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

int main(void)
{
	while(1)
	{
		if(0==init_pipe())
		{
			break;
		}
		sleep(1);
	}
	while(1)
	{
		send_pipe_date();
		sleep(1);
	}
	
	exit_pipe();
	
}
