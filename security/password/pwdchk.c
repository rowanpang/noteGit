#include <stdio.h>
#include <shadow.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <sys/types.h>
#include <pwd.h>

int useage(void)
{
	printf("format error,please usage:\n");
	printf("	pwdchk clpwd [usr]\n");
	printf("	pwdchk -h,show this info message\n");
}

int main(int argc, char **argv)
{
	char *cr=NULL;
	char *pwd_tst=NULL;
	char *usr_tst=NULL;
	char bUseCurUser=0;
	struct spwd  *sp = NULL;
	struct passwd *pwd_info = NULL;

	printf("args num:%d\n",argc);
	if((argc != 2) && (argc != 3)) {
		printf("1 args num:%d\n",argc);
		useage();
		return 0;
	}

	if(!strcmp(argv[1],"-h")) {
		useage();
		return 0;
	} else {
		pwd_tst = argv[1];
	}

	if(argc ==3) {
		usr_tst = argv[2];
		bUseCurUser = 0;
	} else {
		bUseCurUser = 1;
		pwd_info = getpwuid(getuid());
		usr_tst = pwd_info->pw_name;
	}
	printf("user:%s\n",usr_tst);
	sp = getspnam(usr_tst);
	if(sp == NULL) {
		printf("get spentry error\n");
		return -1;
	} else {
		printf("pwd in shadow file:	%s\n",sp->sp_pwdp);
	}
	char len = strlen(sp->sp_pwdp);
	cr = crypt(pwd_tst, sp->sp_pwdp);
	char formatted = 0;
	if (cr == NULL) {
		printf("crypt error\n");
	}
	else {
		printf("pwd test shadow:	");
		char *cr_tmp = cr;
		char *pwdp_tmp = sp->sp_pwdp;
		while(*cr_tmp && *pwdp_tmp) {
			if(*cr_tmp == *pwdp_tmp) {
				len --;
			} else if(!formatted) {
				formatted = 1;
				printf("\033[1;31m");
			}
			printf("%c",*cr_tmp);
			cr_tmp++;
			pwdp_tmp++;
		}
		printf("\033[0m\n");
	}

	printf("compare result:\033[1;31m");
	if(strcmp(sp->sp_pwdp,cr) == 0) {
		printf("ok\n");
	} else {
		printf("ng\n");
	}
	printf("\033[0m");
	//sleep(50);
	return 0;
}
