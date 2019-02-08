#include <ctype.h>
#include <string.h>


#ifdef SHARED_LIB
void module_execute(const char* input_str, char* output_str)
#else
void make_lower(const char* input_str, char* output_str)
#endif
{

	int j;
	unsigned int len = strlen(input_str);
	for(j=0; j<len; j+=1)
	{
		output_str[j] = tolower(input_str[j]);
	}
}

