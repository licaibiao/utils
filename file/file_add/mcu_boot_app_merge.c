/*=============================================================================
#     FileName: CreateFile.cpp
#         Desc: 创建固定大小的文件，然后填充固定的数据
#       Author: Caibiao Lee
#      Version: 
#   LastChange: 2018-11-26 
#      History:
=============================================================================*/
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
 
#define	BOOT_SIZE		(4*1024)
#define APP_SIZE		(28*1024)
#define OUT_FILE_NAME	 "mcu_burn.bin"


int main(int argc, char **argv)
{
	struct stat st;
	FILE *l_pAppFile  = NULL;
	FILE *l_pBootFile = NULL;
	FILE *l_pOutFile  = NULL;
	int 	l_s32Rest = 0;
	int 	l_s32BootSize = 0;
	int 	l_s32AppSize = 0;
	int 	l_s32OutSize = 0;
	unsigned int l_WriteLen = 0;
	unsigned int l_FileLen = 0;
	unsigned char l_u8AddData = 0xff;
	unsigned char l_arru8Buf[1024] = {0xff};
	int l_s32Ret = 0;


	if(3!=argc)
	{
		printf("error:\n");
		printf("usage: %s boot_file app_file \n ", argv[0]);
		printf("eg: %s boot_mcu.bin  boot_app.bin \n ", argv[0]);
		return 0;
	};
 
 	const char *l_pBootName = argv[1];
	const char *l_pAppName = argv[2];
	
	if(NULL==l_pBootName)
	{
		printf("error: input boot file is NULL \n");
		return -1;
	}
	
	if(NULL==l_pAppName)
	{
		printf("error: input app file is NULL \n");
		return -1;
	}
	
	/************input file size check************/
	
	stat(l_pBootName, &st);
	l_s32BootSize = st.st_size;
	
	stat(l_pAppName, &st);
	l_s32AppSize = st.st_size;
	
	if(l_s32BootSize>BOOT_SIZE)
	{
		printf("error: boot size error ;need < %d \n",BOOT_SIZE);
		return -2;
	}
	
	if(l_s32AppSize>APP_SIZE)
	{
		printf("error: app size error ;need < %d \n",APP_SIZE);
		return -2;
	}
	
	/***********write muc boot data********/
	
	l_pOutFile = fopen(OUT_FILE_NAME,"w+");
	if(NULL==l_pOutFile)
	{
		printf("error:create %s error \n",OUT_FILE_NAME);
		return -1;
	}	
	
	l_pBootFile = fopen(l_pBootName,"r+");
	if(NULL==l_pBootFile)
	{
		printf("error:boot file open error \n");
		return -1;
	}
	
	l_s32OutSize = 0;
	while(!feof(l_pBootFile))
	{
		l_s32Ret = fread(l_arru8Buf,1,sizeof(l_arru8Buf),l_pBootFile);
		if(l_s32Ret<0)
		{
			break;
		}
 
		l_s32Ret = fwrite(l_arru8Buf,1,l_s32Ret,l_pOutFile);
		if(l_s32Ret<0)
		{
			break;
		}
		l_s32OutSize+=l_s32Ret;
	}

	/***********add data to need size ********/
	
	if(l_s32OutSize<BOOT_SIZE)
	{
		for(int i=0;i<(BOOT_SIZE-l_s32OutSize);i++)
		{
			l_u8AddData = 0xFF;
			fwrite(&l_u8AddData,1,1,l_pOutFile);	
		}
	}
	
	/*********************write mcu app data****************/
	
	l_pAppFile = fopen(l_pAppName,"r+");
	if(NULL==l_pAppFile)
	{
		printf("error:file %s open error \n",l_pAppName);
		return -1;
	}
	
	while(!feof(l_pAppFile))
	{
		l_s32Ret = fread(l_arru8Buf,1,sizeof(l_arru8Buf),l_pAppFile);
		if(l_s32Ret<0)
		{
			break;
		}
 
		l_s32Ret = fwrite(l_arru8Buf,1,l_s32Ret,l_pOutFile);
		if(l_s32Ret<0)
		{
			break;
		}
		l_s32OutSize+=l_s32Ret;
	}
	
	if(l_pOutFile!=NULL)
	{
		fclose(l_pOutFile);
		l_pOutFile = NULL;
		
	}
	
	if(l_pBootFile!=NULL)
	{
		fclose(l_pBootFile);
		l_pBootFile = NULL;
		
	}
	
	if(l_pAppFile!=NULL)
	{
		fclose(l_pAppFile);
		l_pAppFile = NULL;
		
	}

	return 0;
 
}
 