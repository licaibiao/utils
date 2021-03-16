#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

const unsigned int PORT_FOR_SYSTEM = 9097;

#define MAX_CMD_LEN	1024
#define MAX_STR_RES_LEN		(10*1024)

typedef struct {
	int ret;
	char strResult[MAX_STR_RES_LEN];
}ST_CMD_RESULT;

typedef struct {
	int bNeedResult;
	char strCmd[MAX_CMD_LEN];
}ST_CMD;

static int send_recv(char *cmd, int *ret, char* strResult, int nSize)
{
	struct sockaddr_in sin;
	struct sockaddr_in cin;
	int port = PORT_FOR_SYSTEM;
	socklen_t addr_len;
	int s_fd;
	int n;
	ST_CMD_RESULT stCmdResult = {0};
	fd_set rfds;
    ST_CMD stCmd = {0};

	printf("biao debug %d \n",__LINE__);

    if (0 >= nSize || NULL == strResult)
    	stCmd.bNeedResult = 0;
    else
    	stCmd.bNeedResult = 1;
    strncpy(stCmd.strCmd, cmd, MAX_CMD_LEN);

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, "128.0.0.1", &sin.sin_addr);
	sin.sin_port = htons(port);

	printf("biao debug %d \n",__LINE__);
	s_fd = socket(AF_INET, SOCK_STREAM , 0);
	//s_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(s_fd == -1)
	{
		printf("create socket failed!\n");
		return -1;
	}

	printf("biao debug %d \n",__LINE__);
	n = sendto(s_fd, &stCmd, sizeof(stCmd.bNeedResult) + strlen(stCmd.strCmd) + 1, 0, (struct sockaddr *) &sin, sizeof(sin));
	if(n == -1)
	{
		printf("send failed!\n");
		close(s_fd);
		return -1;
	}
	printf("biao debug %d \n",__LINE__);
	FD_ZERO(&rfds);
	FD_SET(s_fd, &rfds);

	if(stCmd.bNeedResult)
	{
		struct timeval tv_out;
		tv_out.tv_sec = 5;
		tv_out.tv_usec = 0;
		n = select(s_fd+1, &rfds, NULL, NULL, &tv_out);    
	}
	else
	{
		n = select(s_fd+1, &rfds, NULL, NULL, NULL); 
	}

	printf("biao debug %d \n",__LINE__);
	if (n == -1)
	{
		printf("select error!\n");
		close(s_fd);
		return -1;
	}
	else if(n == 0)
	{
		printf("recvfrom timeout!\n");
		close(s_fd);
        return -1;
	}

	addr_len = sizeof(cin);
	n = recvfrom(s_fd, &stCmdResult, sizeof(stCmdResult), 0, (struct sockaddr *) &cin, &addr_len);
	*ret = stCmdResult.ret;
	strncpy(strResult, stCmdResult.strResult, nSize);
	if(n == -1)
	{
		printf("recvfrom failed!\n");
		close(s_fd);
		return -1;
	}

	close(s_fd);

	return 0;
}

int utils_system(char *cmd, ...)
{
	va_list ap;
	char cmdstr[1024] = "";

	va_start(ap, cmd);
	vsnprintf(cmdstr, sizeof(cmdstr), cmd, ap);
	va_end(ap);
	int ret = -1;
	send_recv(cmdstr, &ret, NULL, 0);
	return ret;
}


int main(void)
{
	utils_system("echo hello");
	return 0;
	
}



