#include<stdio.h>
#include<stdlib.h>
#include<wchar.h>
#include<locale.h>
#include<string.h>
#include<malloc.h>
#include<assert.h>

//int main(int argc,char **argv)
int main(void)
{
	//setlocale(LC_ALL,"en_US.UTF-8");
	unsigned char *old_locale, *old_sl;
	//unsigned char *env_locale, *env_sl;
	unsigned char *env_locale;
	
	env_locale = getenv("LANG");
	//unsigned char * const env_sl = strdup(env_locale);
	unsigned char *env_sl_bak=NULL;
	unsigned char *env_sl = malloc(strlen(env_locale) + 1);
	env_sl_bak = env_sl;
	assert(env_sl!= NULL);
	strcpy(env_sl , env_locale);

	if(env_sl == NULL)
	{
		return -1;	
	}	
	//printf("env_locale:%s,env_sl:%s\n",env_locale,env_sl);	
	
	setlocale(LC_ALL,env_sl);
	wprintf(L"---------Unicode Program wchar_t start----------\n");

	char env_sl_len = strlen(env_sl);
	wchar_t *wenv_sl = malloc((env_sl_len + 1) * sizeof(wchar_t));
	wchar_t tmp[1]={L'\0'};
	mbstate_t env_sl_st;
	size_t conv_len = 0;
	memset(&env_sl_st,'\0',sizeof(env_sl_st));
	//wprintf(L"env_sl:0x%x\n",(long int)env_sl);
	//mbsrtowcs(wenv_sl,(const char **)&env_sl,env_sl_len,&env_sl_st);	
	conv_len = mbsrtowcs(wenv_sl,(const char **)&env_sl,env_sl_len,NULL);	
	//conv_len = mbsrtowcs(wenv_sl,(const char **)&env_sl,8,NULL);	
	//conv_len = mbsrtowcs(wenv_sl,(const char **)&env_sl,18,NULL);	
	//conv_len = mbsrtowcs(NULL,(const char **)&env_sl,18,NULL);	
	//conv_len = mbsrtowcs(NULL,(const char **)&env_sl,8,NULL);//if dest null,didn't care len	
	//wprintf(L"after conver: %ls,conv_len:%d env_sl:0x%x\n",wenv_sl,conv_len,(long int)env_sl);

	old_locale = setlocale(LC_ALL,NULL);
	old_sl = strdup(old_locale);
	if(old_sl == NULL)
	{
		return -1;	
	}
	//printf("old_locale:%s,old_sl:%s\n",old_locale,old_sl);	

	wchar_t zh_wc[]=L"ChineseSimplify:测试Unicode编码汉字是否可以正常显示";
	wprintf(L"%ls\n",zh_wc);
	
	free(old_sl);
	free(env_sl_bak);

	wprintf(L"---------Unicode Program wchar_t end-----------\n");
	return 0;
}
