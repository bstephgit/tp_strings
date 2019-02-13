#include <ctype.h>
#include <string.h>

#ifdef SHARED_LIB
void module_execute_n(const char* input_str, char* output_str, size_t len)
#else
void make_lower_n(const char* input_str, char* output_str, size_t len)
#endif
{

	unsigned int j;
	for(j=0; j<len; j+=1)
	{
		output_str[j] = tolower(input_str[j]);
	}
}

#ifdef SHARED_LIB
void module_execute(const char* input_str, char* output_str)
#else
void make_lower(const char* input_str, char* output_str)
#endif
{
	unsigned int len = strlen(input_str);

#ifdef SHARED_LIB
	module_execute_n(input_str,output_str,len);
#else
	make_lower_n(input_str,output_str,len);
#endif
}

