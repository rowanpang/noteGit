#include <stdio.h>
#include <shadow.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>

int main(int argc, char **argv)
{
	   printf("arg1:%s\n",argv[1]);
	   printf("arg2:\"%s\"\n",argv[2]);
	   char hn[250]={'\0'};
  	   gethostname(hn,sizeof(hn));
	   printf("host name :%s\n",hn);
   	
		char *cr=NULL;
		cr=crypt("123","123");
		cr=crypt("123","12IbR.gJ8wcpc");
		//cr=crypt("123","!!");

		if (cr == NULL)
		{
			printf("crypt error\n");
		}	
		else
		{
			printf("crypt result:%s\n",cr);
		}

   		 struct spwd  *sp;
       sp = getspnam(argv[1]);
       sp = getspnam("sxy");
       if(sp == NULL)
       {
              printf("get spentry error\n");
              return -1;
       }
	   else
		{
			printf("getspname_pwd:%s\n",sp->sp_pwdp);
		}	
 
       if(strcmp(sp->sp_pwdp, (char*)crypt(argv[2], sp->sp_pwdp)) == 0)
      {
              printf("yes\n");
       }
       else
       {
              printf("no\n");
       }

	
       return 0; 
}
