#include <stdio.h>
#include <stdarg.h>


int printmsg(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	int res = vprintf(format, ap);

	va_end(ap);
	return res;
}
