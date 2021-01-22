#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

/* 递归调用，输出目录下所有目录及文件*/
void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	
	/* 打开目录 */
	if ((dp = opendir(dir)) == NULL) {
		fprintf(stderr, "Can`t open directory %s\n", dir);
		return ;
	}
	
	/* 切换目录 */
	chdir(dir);
	/* 返回目录流下个目录的进入点 */
	while ((entry = readdir(dp)) != NULL) 
	{
		lstat(entry->d_name, &statbuf);
		/* 判断是否为目录*/
		if (S_ISDIR(statbuf.st_mode)) 
		{
			/* 如果是 . 或.. 目录，结束该次循环 */
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 )
			{
				continue;
			}
			
			/* %*s  这里 * 被depth 代替，做标准输出最小长度控制，输出目录名 */
			printf("%*s%s/\n", depth, "", entry->d_name);
			/* 递归调用 */
			printdir(entry->d_name, depth+4);
		} 
		else
		{
			/* 输出文件名 */
			printf("%*s%s\n", depth, "", entry->d_name);
		}	
	}
	/* 切换到目录 .. */
	chdir("..");
	
	/* 关闭目录 */
	closedir(dp);	
}

int main(int argc, char *argv[])
{
    char *topdir = ".";  
    if (argc >= 2)
	{
		 topdir = argv[1];
	}
       
	printf("Directory scan of %s\n", topdir);
	printdir(topdir, 0);
	printf("done.\n");
	
	exit(0);
}