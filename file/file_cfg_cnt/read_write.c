

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>
#include <sys/types.h>

#define  EMMC_CCNT_PATH  "./config/emmc_cnt"


int read_emmc_format_cnt(void)
{
    int l_s32Read = 0;
	int l_s32Cnt = 0;
    FILE *l_pFd = NULL;

    //打开配置文件，设置系统信息
    l_pFd = fopen(EMMC_CCNT_PATH,"r");
    if(NULL==l_pFd)
    {
		printf("%s open error \n",__FUNCTION__);
        return -1;
    }

    l_s32Read = fread((char *)&l_s32Cnt,1, sizeof(l_s32Cnt),l_pFd);
	if(l_s32Read<0)
	{
		return -2;
	}

    fclose(l_pFd);

	printf("cnt = %d \n",l_s32Cnt);
	
    return l_s32Cnt;		
}

int write_emmc_format_cnt(void)
{
	int l_s32Read = 0;
	int l_s32Cnt = 0;
	FILE *l_pFd = NULL;
	
	l_s32Cnt = read_emmc_format_cnt();
	
	//文件不存在，需要创建文件
	if((l_s32Cnt<0)&&(0==access(EMMC_CCNT_PATH, F_OK)))
	{
		return -1;
	}
	
	l_s32Cnt ++;
	
	l_pFd = fopen(EMMC_CCNT_PATH, "wb");
	if(l_pFd>0)
	{
		l_s32Read = fwrite((char *)&l_s32Cnt,1, sizeof(l_s32Cnt),l_pFd);
		fclose(l_pFd);
	}
	
	return 0;
}


int main(void)
{
	int i = 0;
	
	for(i=0;i<5;i++)
	{
		write_emmc_format_cnt();
	}

	return 0;
	
}


