#include <stdio.h>
#include <stdarg.h>

#include "../inc/utils.h"

int printmsg(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	int res = vprintf(format, ap);

	va_end(ap);
	return res;
}

int charcode(char* str, size_t len)
{
	unsigned int i;
	for (i=0; i<len; i += 1)
	{
		char n[5];
		_itoa((int)str[i],n);

		putchar('[');
		int index=0;
		char ch=n[index];
		while(ch!='\0')
		{
			putchar(ch);
			index += 1;
			ch = n[index];	
		}
		putchar(']');
	}
	return i;
}
