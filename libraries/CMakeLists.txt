add_library(utils STATIC 
						./utils/src/printmsg.c 
						./utils/src/parse.c 
						./utils/src/itoa.c )

add_library(upper_s STATIC	./upper/src/upper.c)
add_library(upper   SHARED	./upper/src/upper.c)


add_library(lower_s STATIC	./lower/src/lower.c)
add_library(lower   SHARED	./lower/src/lower.c)


add_library(rot13_s STATIC	./rot13/src/rot13.c)
add_library(rot13   SHARED	./rot13/src/rot13.c)

target_compile_definitions(upper PRIVATE SHARED_LIB)
target_compile_definitions(lower PRIVATE SHARED_LIB)
target_compile_definitions(rot13 PRIVATE SHARED_LIB)

add_subdirectory(tst ./test)
