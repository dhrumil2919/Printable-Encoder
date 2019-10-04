#include<stdio.h>
#include<string.h>
int main(int argc,char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"Usage: ./encoder shellcode\n");
		return -1;
	}
	int n = strlen(argv[1])/4;
	//printf("Shellcode Size : %d\n",n);
	int k = n;
	n = n + n%2;
	int e_n = ((3*n)/2)+2;
	unsigned char encoded[e_n];	
	char temp[4];
	int i,j;
	j = 0;
	char end = 0x26;
	for(i = 0 ; i < n ; i++)
	{
		temp[0] = argv[1][4*i+2];
		temp[1] = argv[1][4*i+3];
		i++;
		if(i == k)
		{
			temp[2] = '9';
			temp[3] = '0';
		}
		else
		{
			temp[2] = argv[1][4*i+2];
			temp[3] = argv[1][4*i+3];
		}
		unsigned int x = strtoul(temp,0,16);
		unsigned int c = (0x3f & x) + 64; 		//0x30 + ((0xf000 & x) >> 12);
		unsigned int b = ((x >> 6) & 0x3f) + 64; 	//((((0xc00 & x) >> 10) + 0x4) << 4) + ((0x300 & x) >> 6) + ((0xc0 & x) >> 6);
		unsigned int a = ((x >> 12) & 0xf) + 64;	//0x40 + (0x30 & x) + (0xf & x);
	       	encoded[3*j] = --a;
		encoded[3*j+1] = --b;
		encoded[3*j+2] = --c;
		printf("%s ",temp);
		printf("%x %x %x %c %c %c\n",a,b,c,a,b,c);
		j++;
	}
	encoded[3*j] = end;
	encoded[e_n-1] = '\0';
	printf("%s\n",encoded);
	return 0;
}
