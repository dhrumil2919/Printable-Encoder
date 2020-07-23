#define E 164
void handler_function()
{
	int i,j;
	for(i = 0 ,j = E; i < n-1 ; i++,j++)
	{
		if(original[i] != shellcode[j])
		{
			printf("Byte Num %d = %d\n",i,original[i]);
		}
	}
	exit(1);
}
