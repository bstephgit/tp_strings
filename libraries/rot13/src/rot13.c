#include <stdio.h>
#include <string.h>

#ifdef SHARED_LIB
void module_execute(const char* input_str, char* output_str)
#else
void rot13(const char* input_str, char* output_str)
#endif
{

	int j;
	unsigned int len = strlen(input_str);
	for(j=0; j<len; j+=1)
	{
		char c = input_str[j];
		char offset = 0;
	
		if(c>='a' && c<='z')
		{
			offset = 'a';
		}
		if(c>='A' && c<='Z')
		{
			offset = 'A';
		}
		if(offset > 0)
		{
			c = offset + ((c-offset+13)%26);
		}
		output_str[j] = c;
	}
}

