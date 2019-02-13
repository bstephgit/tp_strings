#include <stdio.h>
#include <string.h>

#ifdef SHARED_LIB
void module_execute_n(const char* input_str, char* output_str, size_t len)
#else
void rot13_n(const char* input_str, char* output_str, size_t len)
#endif
{

	unsigned int j;
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


#ifdef SHARED_LIB
void module_execute(const char* input_str, char* output_str)
#else
void rot13(const char* input_str, char* output_str)
#endif
{
	unsigned int len = strlen(input_str);
#ifdef SHARED_LIB
	module_execute_n(input_str, output_str, len);
#else
	rot13_n(input_str, output_str, len);
#endif
}

