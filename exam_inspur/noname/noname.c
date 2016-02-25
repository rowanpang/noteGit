#include <stdio.h>

#if 0
	#define dprintf(fmt,arg...) printf(fmt,##arg)
#else
	#define dprintf(fmt,arg...) 
#endif

int main(int argc,char** argv)
{
	int s,t,n;

	freopen("noname.in","r",stdin);
	scanf("%d %d %d",&s,&t,&n);
	dprintf("%d %d %d\n",s,t,n);
	
	int map[n][n];
	int i,j;
	int start = s;

	for(i=0;i<n;i++){
		for(j=0;j<(n-i);j++){
			map[j][i] = ((start+j)%t == 0)?t:((start+j)%t);
		}
		start = ((map[j-1][i])%t)+1;
		dprintf("start:%d,i:%d,j:%d,map:%d\n",start,i,j,map[j-1][i]);
	}	

	freopen("noname.out","w",stdout);
	for(i=0;i<n;i++){
		for(j=0;j<(n-i-1);j++){
			printf("%d ",map[i][j]); 
		}
		printf("%d",map[i][j]); 
		printf("\n");
	}	
}

