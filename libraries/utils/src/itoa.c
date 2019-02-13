#include <stdio.h>
#include <stdlib.h>

char* _itoa( int n, char to[])
{
	int index = 0;
	char table[] = "0123456789";
		
	int sign;
	int d,mod;

	if(n<0)
	{
		sign = -1;
		n *= -1;
		to[index] = '-';
		index += 1;
	}

	do	
	{
		d = n / 10;
		mod = n % 10;
		
		to[index] = table[mod];
		index += 1;
		n = d;

	}while(n>0);
			
	if (sign<0)
		d = 1;
	else
		d = 0;
	
	to[index]=0;
	index -= 1;	
	while( d < index )
	{
		char temp = to[d];
		to[d]=to[index];	
		to[index]=temp;
		d += 1;
		index -= 1;	
	}
	return to;
}

