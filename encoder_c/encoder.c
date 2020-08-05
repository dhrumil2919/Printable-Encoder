#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(int argc,char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"./encoder \"0xcd0x80..\"(Shellcode) \n");
		exit(1);
	}
	int n = strlen(argv[1])/4;
	int k = n;
	n = n + n%2;
	int e_n = ((3*n)/2)+2;
	unsigned char encoded[e_n];	
	char temp[4];
	int i,j;
	j = 0;
	char end = 0x26;	// & characeter at the end of shellcode
	for(i = 0 ; i < n ; i++)
	{
		temp[0] = argv[1][4*i+2];
		temp[1] = argv[1][4*i+3];
		i++;
		if(i == k)    // Adding NOP instruction if needed
		{
			temp[2] = '9';
			temp[3] = '0';
		}
		else
		{
			temp[2] = argv[1][4*i+2];
			temp[3] = argv[1][4*i+3];
		}
		unsigned int x = strtoul(temp,0,16);  // converting string to int base 16
		unsigned int c = (0x3f & x) + 0x3F; 		
		unsigned int b = ((x >> 6) & 0x3f) + 0x3F; 	
		unsigned int a = ((x >> 12) & 0xf) + 0x3F;
		encoded[3*j] = a;
		encoded[3*j+1] = b;
		encoded[3*j+2] = c;
		j++;
	}
	encoded[3*j] = end;
	encoded[e_n-1] = '\0';
	printf("%s\n",encoded);
	return 0;
}
