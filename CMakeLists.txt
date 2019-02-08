cmake_minimum_required(VERSION 3.5.1)

find_package(CUnit)

message(STATUS "${CUNIT_FOUND} ${CUNIT_INCLUDE_DIRS} ${CUNIT_LIBRARIES}")

add_subdirectory(tools ./bin)
add_subdirectory(libraries ./lib)


