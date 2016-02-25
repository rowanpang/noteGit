#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/xattr.h>
#include <sys/types.h>
void testset()
{
	char key[7] = {'u','s','e','r','.','#','\0'};    
	char value[2] = {'#','\0'};    
	int i, ret;    
	for(i = 0; i < 10; i++)
	{    
		key[5] = value[0] = '0'+i;    
		ret = setxattr("test", key, value, 2, 0);    
	}
}
void testlist()
{    
	char buf[1000];    
	int ret, i=0, j = 0;    
	printf("%s:The key on test are:\n",__FUNCTION__);
	ret = listxattr("test", buf, 1000);
	while(i < ret){
		printf("%s\n", buf+i);
		i += strlen(buf+i);
		i++;
	}
}
void testremove()
{
	char key[7] = "user.2";
	int ret;
	ret = removexattr("test",key);
	printf("%s,%d\n",__FUNCTION__,ret);
}
void testget()
{
	char key[7] = "user.#";
	char value[3];
	int ret, i;
	printf("%s:The <key,value> on test are:\n",__FUNCTION__);
	for(i = 0; i < 11; i++)
	{
		key[5] = '0'+i;
		ret = getxattr("test",key, value, 3);
		if(ret != -1)
		printf("<%s,%s>\n", key, value);
	}
}
void testgetacl()
{
	char value[1000]={0};
	getxattr("test","system.posix_acl_access",value,sizeof(value));
	printf("value:%d\n",value[0]);

}
int main()
{
	testset();
	testlist();
	testremove();
	testget();
	testgetacl();
	return 0;
}
