#include<stdio.h>
int main()
{
	FILE *f;
	f = fopen("bytes","r");
	int i = 0;
	char c;
	fscanf(f,"%c",&c);
	while(!feof(f) && i < 256)
	{
		printf("%c %x\n",c,c);
		fscanf(f,"%c",&c);
		i++;
	}
	return 0;
}
