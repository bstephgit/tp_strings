#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "lower.h"
#include "upper.h"
#include "rot13.h"
#include "test.h"


void test_make_lower()
{

	CU_LOWER_TEST("ABC","abc");
	CU_LOWER_TEST("ABC+09@","abc+09@");
	CU_LOWER_TEST("ABCde","abcde");
	CU_LOWER_TEST("ABC123xyz","abc123xyz");
	CU_LOWER_TEST("1234","1234");
}
	
void test_make_lower_n()
{
	CU_LOWER_N_TEST("ABCD","abc",3);
	CU_LOWER_N_TEST("1AB2CD","1ab2c",5);
	CU_LOWER_N_TEST("AB+D","ab+d",4);
	CU_LOWER_N_TEST("ABCD","abcD",3);
	CU_LOWER_N_TEST("ABC1","abc1",3);
	CU_LOWER_N_TEST("@Bc1","@bc1",4);
}

void test_make_upper()
{
	CU_UPPER_TEST("abc","ABC");
	CU_UPPER_TEST("abc+09@","ABC+09@");
	CU_UPPER_TEST("abcde","ABCDE");
	CU_UPPER_TEST("abc123xyz","ABC123XYZ");
	CU_UPPER_TEST("1234","1234");
}
	
void test_make_upper_n()
{
	CU_UPPER_N_TEST("abcd","ABC",3);
	CU_UPPER_N_TEST("1ab2cD","1AB2C",5);
	CU_UPPER_N_TEST("ab+d","AB+D",4);
	CU_UPPER_N_TEST("abcd","ABCd",3);
	CU_UPPER_N_TEST("abc1","ABC1",3);
	CU_UPPER_N_TEST("@bC1","@BC1",4);
}
	
void test_rot13()
{
	CU_ROT13_TEST("ABC","NOP");
	CU_ROT13_TEST("ABC+09@","NOP+09@");
	CU_ROT13_TEST("ABCde","NOPqr");
	CU_ROT13_TEST("ABC123xyz","NOP123klm");
	CU_ROT13_TEST("1234","1234");
}
	
void test_rot13_n()
{
	CU_ROT13_N_TEST("ABCD","NOP",3);
	CU_ROT13_N_TEST("1AB2CD","1NO2P",5);
	CU_ROT13_N_TEST("AB+D","NO+Q",4);
	CU_ROT13_N_TEST("ABCD","NOPD",3);
	CU_ROT13_N_TEST("nop1","abc1",3);
	CU_ROT13_N_TEST("@Bc1","@Op1",4);
	CU_ROT13_N_TEST("z@Bc1","m@Oc1",3);
}

int main()
{
	
	CU_initialize_registry();

	CU_pSuite suite = CU_add_suite("lower library",NULL,NULL);
	CU_add_test(suite,"lower function", test_make_lower);
	CU_add_test(suite,"lower function N", test_make_lower_n);
	CU_add_test(suite,"upper function", test_make_upper);
	CU_add_test(suite,"upper function N", test_make_upper_n);
	CU_add_test(suite,"rot13 function", test_rot13);
	CU_add_test(suite,"rot13 function N", test_rot13_n);

	CU_basic_run_suite(suite);

	CU_cleanup_registry();

	return 0;
}
