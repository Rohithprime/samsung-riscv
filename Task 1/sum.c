#include<stdio.h>

void main()
{
	int sum = 0,n = 10,i = 1;
	while(i != n+1)
	{
		sum += i;
		i++;
	}
	printf("sum of first %d numbers is %d\n",n,sum);    

}
