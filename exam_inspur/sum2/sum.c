#include <stdio.h>

int main()
{
	int a,i,s,n;
	freopen("sum.in","r",stdin);
	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		scanf("%d",&a);
		s=s+a;
	}
	freopen("sum.out","w",stdout);
	printf("%d\n",s);
	return 0;
}
