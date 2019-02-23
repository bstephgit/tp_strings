#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "utils.h"


void test_itoa()
{

	char buf[32];

	CU_ASSERT_STRING_EQUAL(_itoa(15,buf),"15")
	CU_ASSERT_STRING_EQUAL(_itoa(-15,buf),"-15")
	CU_ASSERT_STRING_EQUAL(_itoa(0,buf),"0")
	CU_ASSERT_STRING_EQUAL(_itoa(2457,buf),"2457")
	CU_ASSERT_STRING_EQUAL(_itoa(2457.5,buf),"2457")
	CU_ASSERT_STRING_EQUAL(_itoa(-2457,buf),"-2457")
	CU_ASSERT_STRING_EQUAL(_itoa(-2457.5,buf),"-2457")
}

void test_parse_to()
{
	char* str = "1234%@45678?>12345.....6789";
	char* end = str + strlen(str);
	char* to;

	to = parse_to(str,end,"/%");
	CU_ASSERT_PTR_EQUAL(to,str+4);

	to = parse_to(str,end,"867");
	CU_ASSERT_PTR_EQUAL(str+8,to);

	to = parse_to(str,end,"t.o");
	CU_ASSERT_PTR_EQUAL(str+18,to);

	to = parse_to(str,end,"+*");	
	CU_ASSERT_PTR_EQUAL(to,end);
	
}

void test_parse_after()
{
	
	char* str = "1234%@45678?>12345.....6789";
	char* end = str + strlen(str);
	char* to;

	to = parse_after(str,end,"1234");
	CU_ASSERT_PTR_EQUAL(str+4,to);

	to = parse_after(str,end,"123456%@");
	CU_ASSERT_PTR_EQUAL(str+9,to);
	
	to = parse_after(str,end,"123456789%@?>.");
	CU_ASSERT_PTR_EQUAL(to,end);
	
}

int main()
{
	
	CU_initialize_registry();

	CU_pSuite suite = CU_add_suite("utils library",NULL,NULL);
	CU_add_test(suite,"_itoa function", test_itoa);
	CU_add_test(suite,"parse_to function", test_parse_to);
	CU_add_test(suite,"parse_after function", test_parse_after);
	CU_basic_run_suite(suite);

	CU_cleanup_registry();

	return 0;
}
