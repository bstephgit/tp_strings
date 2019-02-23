
add_executable(testlibs ./TestLibs/main.c) 
target_include_directories(testlibs PRIVATE
	../lower/inc
	../upper/inc
	../rot13/inc
)
target_link_libraries(testlibs
		upper_s 
		lower_s 
		rot13_s 
		cunit)

add_test(NAME TestLibs COMMAND ./testlibs)
set_tests_properties(TestLibs PROPERTIES FAIL_REGULAR_EXPRESSION "ASSERT")

add_executable(testutils ./TestUtils/main.c) 
target_include_directories(testutils PRIVATE
	../utils/inc
)
target_link_libraries(testutils 
		utils	
		cunit)

add_test(NAME TestUtils COMMAND ./testutils)
set_tests_properties(TestUtils PROPERTIES FAIL_REGULAR_EXPRESSION "ASSERT")

