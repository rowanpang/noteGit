/*=============================================================================
#     FileName: devel.c
#         Desc: /etc/hosts file parser
#       Author: Rowan Pang
#        Email: pangweizhen.2008@hotmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2016-06-13 16:30:09
#      History:
=============================================================================*/
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc,char** argv)
{
	char nm[30]={0};
	int ret;

	if(gethostname(nm,sizeof(nm))){
		perror("get hostname error");
		ret = -1;
		goto out;
	}else{
		printf("host name: %s\n",nm);
	}

	struct hostent *he=NULL;
	sethostent(1);
	do{
		he = gethostent();
		if(!he){
			break;
		}
		printf("hostent->h_name:%s\n",he->h_name);
		char **aliases = NULL;
		for(aliases = he->h_aliases; *aliases; aliases++){  
			printf("\talternate name: %s\n", *aliases);  
		}  
		char **addr_list = NULL;
		char addr_p[30]={0};
		char *dst;
		printf("\thostent->h_length:%d\n",he->h_length);
		if(he->h_addrtype == AF_INET){  
			printf("\taddress type: AF_INET\n");  
			for(addr_list = he->h_addr_list; *addr_list; addr_list++){  
				printf("\taddress: %s\n", inet_ntop(AF_INET, *addr_list, addr_p, INET_ADDRSTRLEN));  
				/*dst = inet_ntop(AF_INET, *addr_list, addr_p, INET_ADDRSTRLEN);*/
				/*printf("ret:%#p,%#x,dst:%#p\n", dst,*dst,addr_p);  */
				/*printf("address: %s\n", addr_p);  */
			} 
		}else if(he->h_addrtype == AF_INET6){  
			printf("\taddress type: AF_INET6\n");  
			printf("\taddress: %s\n", inet_ntop(AF_INET, *addr_list, addr_p, INET6_ADDRSTRLEN));  
		}
		else{  
			printf("\tNot an IPv4/v6 address.\n");  
		}  	
		
		printf("\n");
	}while(1);	

	/*printf("hostent->h_addr_list:%x\n",*((int *)(he->h_addr_list[0])));*/

	ret = 0;
out:
	return;
}
