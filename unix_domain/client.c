#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PATH    "./tmp/server"     
#define HEARBEAT_PAHT  "./tmp/hearbeat"  
#define SYSTEM_PAHT    "./tmp/system_cmd"   

int client_connect_to_server(int *s32SocketId,unsigned char * pu8Path)
{
    int l_s32Len = 0;
    int l_s32Res = 0;
    int l_s32SockFd = 0;
    struct sockaddr_un l_stServerAddr;
    struct sockaddr_un l_stClientAddr;
    char l_as8ClientPath [64] = {0};

    int ret = 0;
    int optval;
    int optlen = sizeof(int);


    if((NULL==pu8Path)||(NULL==s32SocketId))
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }
    
    snprintf(l_as8ClientPath,sizeof(l_as8ClientPath),"%s",pu8Path);

    /**创建socket，指定通信协议为AF_UNIX,数据方式SOCK_STREAM**/
    if((l_s32SockFd = socket(AF_UNIX, SOCK_STREAM|SOCK_NONBLOCK, 0))==-1)
    {
        if(l_s32SockFd>0)
        {
            close(l_s32SockFd);
            l_s32SockFd = 0;
        }
        perror("socket");
        return -1;
    }

    /**一般显式调用bind函数，以便服务器区分不同客户端  
    它也可以不用bind,直接连接**/ 
    memset(&l_stClientAddr, 0, sizeof(l_stClientAddr));  
    l_stClientAddr.sun_family = AF_UNIX;  
    strcpy(l_stClientAddr.sun_path, l_as8ClientPath);  
    l_s32Len = sizeof(l_stClientAddr);
    unlink(l_as8ClientPath);  
    if(bind(l_s32SockFd, (struct sockaddr *)&l_stClientAddr, l_s32Len) < 0) 
    {  
        perror("bind error");
        if(l_s32SockFd>0)
        {
            close(l_s32SockFd);
            l_s32SockFd = 0;
        }
        return -1;
    } 

    /**配置server_address**/
    l_stServerAddr.sun_family = AF_UNIX;
    strcpy(l_stServerAddr.sun_path, SERVER_PATH);
    l_s32Len = sizeof(l_stServerAddr);

    l_s32Res = connect(l_s32SockFd, (struct sockaddr *)&l_stServerAddr, l_s32Len);
    if(l_s32Res == -1) 
    {
        printf("ensure the server is up\n");
        perror("connect");
        if(l_s32SockFd>0)
        {
            close(l_s32SockFd);
            l_s32SockFd = 0;
        }
        return -1;
    }

    *s32SocketId = l_s32SockFd;
    printf("unix domain addr:%d connet success \n",*s32SocketId);
    return 0;
}


int client_send_data(int s32SocketId,unsigned char *pu8Data,int s32Len)
{
    int l_s32Res = 0;
    int l_s32Count = 3;

    if(s32SocketId<=0)
    {
        printf("socke id is error \n");
        return -1;
    }

    if(NULL==pu8Data)
    {
        printf("input para is error \n");
        return -2;
    }

    l_s32Res = write(s32SocketId, pu8Data, s32Len);
    if(l_s32Res!=s32Len)
    {
        while((l_s32Count--)>0)
        {
            usleep(1000*100);
            l_s32Res = write(s32SocketId, pu8Data, s32Len);
            if(l_s32Res==s32Len)
            {
                return 0;
            }

            if(EAGAIN!=errno)
            {
                printf("Send Msg Error l_s32Res = %d errno = %d \n",l_s32Res,errno);			
                return -1;
            }
        }
        printf("Try again, msg send throw away!! \n");
        return 1;
    }

    return 0;
}

int client_exit(int *s32SocketId)
{
    if(*s32SocketId>0)
    {
        close(*s32SocketId);
        *s32SocketId = -1;
    }
}

int client_system(void)
{
    char l_s32Cnt = 0;
    int  l_s32Ret = 0;
    int  s32SocketId = 0;
    unsigned char * pData = "echo hello";
    
    for(l_s32Cnt=0;l_s32Cnt<10;l_s32Cnt++)
    {
        l_s32Ret = client_connect_to_server(&s32SocketId,SYSTEM_PAHT);
        if((l_s32Ret==0)&&(s32SocketId>0))
        client_send_data(s32SocketId,pData,strlen(pData));
        client_exit(&s32SocketId);
        usleep(100000);
    }
}


void *client_hearbeat(void* arg)
{
    char l_s32Cnt = 0;
    int  l_s32Ret = 0;
    int  s32SocketId = 0;
    unsigned char * pData = "live";
    
    while(1)
    {
        l_s32Ret = client_connect_to_server(&s32SocketId,HEARBEAT_PAHT);
        if((l_s32Ret==0)&&(s32SocketId>0))
        {
            break;
        }
        sleep(1);
    }

    while(1)
    {
        l_s32Ret = client_send_data(s32SocketId,pData,strlen(pData));
        if(l_s32Ret <0)
        {
            break;
        }
        sleep(1);   
    }

    client_exit(&s32SocketId);

}


int main(void)
{
    pthread_t l_UnixConnectId;
 
    /**创建unix domain socket连接线程**/
    if(0==pthread_create(&l_UnixConnectId,NULL,client_hearbeat,NULL))
    {
        printf("create unix domin socket lpthread success\n");
    }else
    {
        printf("create unix domin socket lpthread error !!!\n");
    }

    client_system();
    
    pthread_join(l_UnixConnectId,NULL);

    return 0;
}

