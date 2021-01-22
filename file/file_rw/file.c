#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

//#define FILE_NAME	"/config/common/cfg_params.conf"
#define FILE_NAME	"cfg_params.conf"



typedef struct PARK_CFG
{
    unsigned char u8SelfHour;        //上次自检的时间  小时
    unsigned char u8SelfMinute;      //上次自检的时间分钟
    unsigned char u8SelfTestTime;    //后台设置自检周期 单位为分钟
    unsigned char u8EvidenceHour;    //上次取证的时间 小时
    unsigned char u8EvidenceMinute;  //上次取证的时间 分钟
    unsigned char u8EvidenceTime;    //后台设置的取证时间间隔 单位分钟
    unsigned char u8LPMBeginHour;    //后台设置的低功耗开始时间 小时
    unsigned char u8LPMBeginMinute;  //后台设置的低功耗开始时间 分钟
    unsigned char u8LPMEndHour;      //后台设置的低功耗结束时间 小时
    unsigned char u8LPMEndMinute;    //后台设置的低功耗结束时间 分钟
    unsigned char u8ReportNeedCnt;    //雨天模式需要拍照的次数
    unsigned char u8ReportAlreadyCnt; //雨天模式已经拍照的次数
    unsigned char u8ReportTimeMinute; //雨天模式检测周期，时间分钟
    unsigned char u8PlatformSetMode;  //平台设置运行模式 1 为雨天模式
    unsigned char u8HisiLastState;    //海思设备上一个运行状态   
    unsigned char u8HisiCurState;     //海思设备当前运行的状态
    unsigned char u8SerialId;         //备用出入库记录关联id
    unsigned char arru8eeData[5];
    unsigned char arru8igndata[8];
    unsigned char arru8Threshold[18];
}PARK_CFG_T;


int Sdk_fread(char *path, char *filebuf,int maxSize)
{
    FILE *fp=NULL;
    int l_s32len = 0;

    fp=fopen(path,"rb");
    if(fp){
        fseek(fp, 0, SEEK_END);
        int l_s32size = ftell(fp);
        rewind(fp);
        if(l_s32size > maxSize)
        {
            l_s32size = maxSize;
        }
        l_s32len = fread(filebuf, 1, l_s32size, fp);  
        fclose(fp);
     }
    return l_s32len;
}









int main(int argc, char *argv[])
{

	unsigned char Buf[1024] = {0};

	PARK_CFG_T test;


	FILE* l_pF = NULL;
	int l_s32Ret = 0;

#if 0
	l_pF = fopen(FILE_NAME,"rb");
	if(l_pF==NULL)
	{
		printf("open error\n");
	}else
	{
		printf("open ok\n");
		
	}

	l_s32Ret = fread(Buf,1,sizeof(Buf),l_pF);
	if(l_s32Ret>0)
	{
		memcpy(&test,Buf,sizeof(Buf));

		printf("test.u8SerialId = %d  %d\n",test.u8SerialId,test.u8HisiCurState);

	}

	fclose(l_pF);

#endif
	l_s32Ret = Sdk_fread("aa",Buf,sizeof(Buf));

	 printf("l_s32Ret = %d \n",l_s32Ret);

	return 0;

}
