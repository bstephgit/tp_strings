add_library(printmsg STATIC ./src/printmsg.c)

add_executable(tps ./src/main.c)
target_link_libraries(tps printmsg upper_s lower_s rot13_s dl)
target_include_directories(tps PRIVATE  ./inc 
					../libraries/upper/inc
					../libraries/lower/inc
					../libraries/rot13/inc
)
