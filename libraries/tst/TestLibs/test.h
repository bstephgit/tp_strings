
#define CU_FUNC_TEST(f,s1,s2) \
	{ \
			char* buffer = (char*) malloc ( (strlen(s2)+1) * sizeof(char) ); \
			f(s1,buffer); \
			CU_ASSERT_STRING_EQUAL( buffer, s2 ) \
			free(buffer); \
	}

#define CU_FUNC_N_TEST(f,s1,s2,n) \
	{ \
			char* buffer = (char* ) malloc( (strlen(s2)+1) * sizeof(char) ); \
	\
			strcpy(buffer,s2); \
			f(s1,buffer,n); \
	\
			CU_ASSERT_NSTRING_EQUAL( buffer, s2, n ); \
	\
			if(strlen(buffer) > n && strlen(s1) > n){ \
				size_t len = strlen(s1) > strlen(buffer) ? strlen(buffer) : strlen(s1); \
				CU_ASSERT_NSTRING_EQUAL( s1+n, buffer+n, len-n ) \
			} \
	\
			free(buffer); \
	}




#define CU_LOWER_TEST(s1,s2) \
	CU_FUNC_TEST(make_lower,s1,s2)	


#define CU_LOWER_N_TEST(s1,s2,n) \
	CU_FUNC_N_TEST(make_lower_n,s1,s2,n)	


#define CU_UPPER_TEST(s1,s2) \
	CU_FUNC_TEST(make_upper,s1,s2)	


#define CU_UPPER_N_TEST(s1,s2,n) \
	CU_FUNC_N_TEST(make_upper_n,s1,s2,n)	


#define CU_ROT13_TEST(s1,s2) \
	CU_FUNC_TEST(rot13,s1,s2)	


#define CU_ROT13_N_TEST(s1,s2,n) \
	CU_FUNC_N_TEST(rot13_n,s1,s2,n)	
