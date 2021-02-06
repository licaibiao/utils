#include<stdarg.h>
#include<stdio.h>

int sum(int num_args, ...)
{
   int val = 0;
   va_list ap;
   int i;

   va_start(ap, num_args);
   for(i = 0; i < num_args; i++)
   {
      val += va_arg(ap, int);
   }
   va_end(ap);
 
   return val;
}

int sum_debug(void)
{
   printf("10、20 和 30 的和 = %d\n",  sum(3, 10, 20, 30) );
   printf("4、20、25 和 30 的和 = %d\n",  sum(4, 4, 20, 25, 30) );

}



int utils_system(char *cmd, ...)
{
	va_list ap;
	char cmdstr[1024] = "";

	va_start(ap, cmd);
	vsnprintf(cmdstr, sizeof(cmdstr), cmd, ap);
	
	printf("cmdstr = %s \n",cmdstr);

	va_end(ap);

	return 0;
}

int utils_system_debug(void)
{
	utils_system("echo hello""test");
	
	return 0;

}




int main(void)
{
	utils_system_debug();

   return 0;
}

