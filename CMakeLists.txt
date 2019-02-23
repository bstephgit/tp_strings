cmake_minimum_required(VERSION 3.5.1)

include(CTest)

if(MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -pedantic -Werror)
	if(DEBUG)
		add_compile_options(-g)
	endif(DEBUG)
endif()

find_package(CUnit)

message(STATUS "${CUNIT_FOUND} ${CUNIT_INCLUDE_DIRS} ${CUNIT_LIBRARIES}")

add_subdirectory(tools ./bin)
add_subdirectory(libraries ./lib)

enable_testing()

