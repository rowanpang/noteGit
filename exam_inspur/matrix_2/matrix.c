#include <stdio.h>

#if 1
	#define dprintf(fmt,arg...) printf(fmt,##arg)
#else
	#define dprintf(fmt,arg...)
#endif

#define MAX(a,b) (a)>(b)?(a):(b)
#define MIN(a,b) (a)<(b)?(a):(b)

int min(int a,int b,int c)
{
	if(a < b)
		return a<c?a:c;
	else
		return b<c?b:c;
}

int main(int argc,char** argv)
{
	int nRow,nColumn;
	freopen("matrix.in","r",stdin);
	scanf("%d %d",&nRow,&nColumn);
	dprintf("%d %d\n",nRow,nColumn);
	int map[nRow][nColumn];
	int i,j;
	for(i=0;i<nRow;i++){
		for(j=0;j<nColumn;j++){
			scanf("%d",&map[i][j]);	
			dprintf("%d ",map[i][j]);
		}
		dprintf("\n");
	}

	int maxLength = 0;
	for(i=1;i<nRow;i++){
		for(j=1;j<nColumn;j++){
			if(map[i][j]!=0)
			{
				map[i][j] = min(map[i-1][j],map[i-1][j-1],map[i][j-1]) + map[i][j];
				maxLength = MAX(maxLength,map[i][j]);
			}
		}
	}

	dprintf("maxLength:%d\n",maxLength);
	return 1;
}
