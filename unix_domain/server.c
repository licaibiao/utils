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
#include <signal.h>
#include <pthread.h>

#define SERVER_PATH    "./tmp/server"     
#define HEARBEAT_PAHT  "./tmp/hearbeat"  
#define SYSTEM_PAHT    "./tmp/system_cmd"  

#define PROCESS_NAME    "main"

static unsigned char gs_u8HearbeatFlag = 0;


typedef struct THREAD_PARA
{
    int s32SocketId;
    struct sockaddr_un stClientAddr;
}THREAD_PARA_S;

void stop(int signo)
{
    int i = 0;
    printf(" stop \n");
    exit(0);
}


void signal_pipe(int signo)
{

}

int check_client_status(char * name)
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


int set_system_log(void)
{
#if 0
    system("dmesg > /config/dmesg.log");
    system("date >> /config/dmesg.log");
    system("logcat -d > /config/logcat.log");
    system("date >> /config/logcat.log");
    sync();
#else
    printf("%s %d out\n",__FUNCTION__,__LINE__);
    return 0;
#endif

}

int set_heartbeat_log(void)
{
#if 0
    system("echo \"heartbeat no recv \" >> /config/heartbeat.log");
    system("date >> /config/heartbeat.log");
    sync();
#else
    printf("%s %d out\n",__FUNCTION__,__LINE__);
    return 0;

#endif
    
}




int server_read_data(int *s32SocketId,unsigned char *pu8DataAddr,int s32MaxLen,int *ps32Datalen)
{
    int l_s32Res       = 0;
    int l_s32SocketId  = 0;
    fd_set l_stfds;
    struct timeval timeout={1,50};
    
    if((NULL==s32SocketId)||(*s32SocketId<=0)||(NULL==ps32Datalen))
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }

    if((NULL==ps32Datalen)||(s32MaxLen<=0))
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -2; 
    }

    l_s32SocketId  = *s32SocketId;

    memset(pu8DataAddr,0,s32MaxLen);

    /**判断客户端是否已经关闭**/
    FD_ZERO(&l_stfds);
    FD_SET(l_s32SocketId,&l_stfds);
    l_s32Res = select(l_s32SocketId+1,&l_stfds,NULL,NULL,&timeout);
    if(l_s32Res > 0)
    {
        l_s32Res=read(l_s32SocketId,pu8DataAddr,s32MaxLen);
        if((l_s32Res>0)&&(l_s32Res<=s32MaxLen))
        {
            *ps32Datalen  = l_s32Res;
            return 0;
        } else
        {
            if(errno == EINTR)
            {
                printf("Interrupted by the signal!\n");
                return -3;
            } 
            else
            {
                /**连接的对方已经关闭**/
                printf("%s %d error!,the sockfd maybe closed fd =%d !\n",
                __FUNCTION__,__LINE__,l_s32SocketId);
                close(l_s32SocketId);
                *s32SocketId = 0;
                return -4;
            }
        }

    }else if(0==l_s32Res)
    {
        printf("%s %d the timeout! content normal\n",__FUNCTION__,__LINE__);
        return 0;
    }else if (errno == EINTR)
    {
        printf("Interrupted by the signal!\n");
        return -5;
    }else 
    {
        printf("select error!,try again!\n");
        return -6;
    }

    return 0;

}

int server_hearbeat_handle(int SocketId)
{
    int l_s32Len = 0;
    int l_s32Res = 0;
    int l_s32ErrorCnt = 0;
    int l_s32Status = 0;
    unsigned char l_arru8Buf[128] = {0};

    
    printf("%s %d \n",__FUNCTION__,__LINE__);

    gs_u8HearbeatFlag = 1;
    while(1)
    {
        l_s32Res = server_read_data(&SocketId,l_arru8Buf,sizeof(l_arru8Buf),&l_s32Len);
        if((0==l_s32Res)&&(l_s32Len>0))
        {
            
            printf("%s \n",l_arru8Buf);
        }

        if((0!=l_s32Res)&&(0==l_s32Len))
        {
            l_s32ErrorCnt ++;
        }else
        {
            l_s32ErrorCnt = 0;
        }

        if(l_s32ErrorCnt > 20)
        {
            set_heartbeat_log();
            exit(0); //退出该进程
        }


        l_s32Len = 0;
        memset(l_arru8Buf,0,sizeof(l_arru8Buf));
        sleep(1);
    } 
}

int server_system_handle(int SocketId)
{
    int l_s32Len = 0;
    int l_s32Res = 0;
    unsigned char l_arru8Buf[256] = {0};

    l_s32Res = server_read_data(&SocketId,l_arru8Buf,sizeof(l_arru8Buf),&l_s32Len);

    if((l_s32Res==0)&&(l_s32Len>0))
    {
        printf("%s\n",l_arru8Buf);
        system(l_arru8Buf);
    }

    if(SocketId>0)
    {
        close(SocketId);
        SocketId = -1;
    }
}

void* server_data_handle(void* arg)
{
    THREAD_PARA_S* l_stPara;

    l_stPara = (THREAD_PARA_S*)arg;

    if(l_stPara->s32SocketId<=0)
    {
        printf("%s %d input para error\n",__FUNCTION__,__LINE__);
        return NULL ;
    }

    printf("client path: %s \n",l_stPara->stClientAddr.sun_path);   
    if(0==strcmp(l_stPara->stClientAddr.sun_path,HEARBEAT_PAHT))
    {
        server_hearbeat_handle(l_stPara->s32SocketId);
    }else if(0==strcmp(l_stPara->stClientAddr.sun_path,SYSTEM_PAHT))
    {
        server_system_handle(l_stPara->s32SocketId);
    }else
    {

    }
}

void* server_accept_pthread(void* arg)
{
    int i = 0;
    int l_s32ServerSockFd = 0;
    int l_s32ClientSockFd = 0;
    int l_s32ServerLen = 0;
    int l_s32ClientLen = 0;
    pthread_t l_stClientId = 0;

    int ret = 0;
    int optval;
    int optlen = sizeof(int);

    struct sockaddr_un l_stServerAddr;
    struct sockaddr_un l_stClientAddr;

    THREAD_PARA_S l_stPara;

    /**解除原有server_socket对象链接**/
    unlink(SERVER_PATH);	

    /**创建socket，指定通信协议为AF_UNIX,数据方式SOCK_STREAM**/
    l_s32ServerSockFd = socket(AF_UNIX, SOCK_STREAM|SOCK_NONBLOCK, 0);

    /**配置l_stServerAddr**/
    l_stServerAddr.sun_family = AF_UNIX;
    strcpy(l_stServerAddr.sun_path, SERVER_PATH);
    l_s32ServerLen = sizeof(l_stServerAddr);

    bind(l_s32ServerSockFd, (struct sockaddr *)&l_stServerAddr, l_s32ServerLen);

    /**最大监听10个客户端**/
    listen(l_s32ServerSockFd, 50);

    printf("server waiting for unix domain client connect\n");
    l_s32ClientLen = sizeof(l_stClientAddr);

    /**接收客户端的链接**/
    while(1)
    {
        l_s32ClientSockFd = 0;
        l_s32ClientSockFd = accept(l_s32ServerSockFd,(struct sockaddr *)&l_stClientAddr,\
        (socklen_t *)&l_s32ClientLen);

        if(-1==l_s32ClientSockFd)
        {
            //printf("accept socket error\n");
            usleep(100000);
            continue;
        }

        l_stPara.s32SocketId = l_s32ClientSockFd;
        l_stPara.stClientAddr = l_stClientAddr;

        if(0==pthread_create(&l_stClientId,NULL,server_data_handle,&l_stPara))
        {
            //printf("create client data handle thread ok !!!\n");	
        }else
        {
            printf("create client data handle thread error !!!\n");	
        }

        /**将客该SockFd与对应的客户端建立映射关系**/
        //printf("unix domain socket patch = %s \n",l_stClientAddr.sun_path);
         usleep(10000);
    }

    if(l_s32ServerSockFd>0)
    {
        close(l_s32ServerSockFd);
        l_s32ServerSockFd = 0;
    }

    unlink(SERVER_PATH);
}

int main(void)
{
    int l_s32Para;
    int l_s32Status;
    int l_s32HBLostCnt = 0;
    int l_s3ClientDownCnt = 0;
    pthread_t l_UnixConnectId;

    /**注册 SIGPIPE信号**/
    signal(SIGPIPE,signal_pipe); 

    /**注册SIGINT 信号**/
    signal(SIGINT,stop); 

    /**创建unix domain socket连接线程**/
    if(0==pthread_create(&l_UnixConnectId,NULL,server_accept_pthread,&l_s32Para))
    {
        printf("create unix domin socket lpthread success\n");
    }else
    {
        printf("create unix domin socket lpthread error !!!\n");
    }

    while(1)
    {

        if(0==gs_u8HearbeatFlag)
        {
            l_s32HBLostCnt ++;
        }else
        {
            l_s32HBLostCnt = 0;
        }

        l_s32Status = check_client_status(PROCESS_NAME);
        
        if(l_s32Status>0) //正常运行
        {
        
        }else if(l_s32Status==0) //程序挂了
        {
            l_s3ClientDownCnt ++;
        }else //参数不对
        {
        
        }
        
        if(l_s32HBLostCnt > 20)
        {
            set_heartbeat_log();
            exit(0); //退出该进程
        }

        if(l_s3ClientDownCnt>5)
        {
            set_system_log();
            exit(0); //退出该进程

        }
        sleep(1);
    }


    /**阻塞，直到线程退出才能退出**/
    pthread_join(l_UnixConnectId,NULL);

    printf("server logout \n");

}




