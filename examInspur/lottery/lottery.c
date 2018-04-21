#include <stdio.h>

int main(int argc,char** argv)
{	
	unsigned int luck[7]={0};
	unsigned int tmp[7]={0};
	unsigned int nmTicket=0;
	unsigned int l[7]={0};

	freopen("lottery.in","r",stdin);
	scanf("%d",&nmTicket);
	scanf("%d %d %d %d %d %d %d",&luck[0],&luck[1],&luck[2],&luck[3],&luck[4],&luck[5],&luck[6]);
	printf("%d,%d,%d,%d,%d,%d,%d\n",luck[0],luck[1],luck[2],luck[3],luck[4],luck[5],luck[6]);
	
	int i,j,k,ret;
	for(i=0;i<nmTicket;i++)
	{
		ret = 0;
		scanf("%d %d %d %d %d %d %d",&tmp[0],&tmp[1],&tmp[2],&tmp[3],&tmp[4],&tmp[5],&tmp[6]);
		printf("%d,%d,%d,%d,%d,%d,%d\n",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6]);
		for(j=0;j<7;j++)
		{
			for(k=0;k<7;k++)
			{
				if(luck[j] == tmp[k])
					ret++;
			}
		}
		switch(ret)
		{
			case 1:l[6]++;break;
			case 2:l[5]++;break;
			case 3:l[4]++;break;
			case 4:l[3]++;break;
			case 5:l[2]++;break;
			case 6:l[1]++;break;
			case 7:l[0]++;break;
			default:break;
		}
	}
	printf("nmTicket:%d\n",nmTicket);
	
	freopen("lottery.out","w",stdout);
	for(i=0;i<6;i++)
		printf("%d ",l[i]);	
	printf("%d",l[i]);	
}

