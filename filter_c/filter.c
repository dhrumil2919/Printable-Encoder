#include<stdio.h>
#include<string.h>
#include<ctype.h>
#define L 512
unsigned char shellcode[L];
int main()
{
if(fgets(shellcode,L-1,stdin) == NULL)
	return 0;
int n = strlen(shellcode);
shellcode[n-1] = '\0';
for(int i = 0 ; i < n-1 ; i++)
	if(!isprint(shellcode[i]))
		printf("Not Printable %d\n",i);
printf("\nShellcode Length: %d\n", strlen(shellcode));
printf("\nShellcode: %s\n",shellcode);
int (*ret)() = (int(*)())shellcode;
ret();
}
