#include <ctype.h>
#include <string.h>

#ifdef SHARED_LIB
void module_execute(const char* input_str, char* output_str)
#else
void make_upper(const char* input_str, char* output_str)
#endif
{

	unsigned int j;
	unsigned int len = strlen(input_str);
	for(j=0; j<len; j+=1)
	{
		output_str[j] = toupper(input_str[j]);
	}
}

#ifdef SHARED_LIB
void module_execute_n(const char* input_str, char* output_str, size_t len)
#else
void make_upper_n(const char* input_str, char* output_str, size_t len)
#endif
{
	unsigned int j;
	for(j=0; j<len; j+=1)
	{
		output_str[j] = toupper(input_str[j]);
	}
}
