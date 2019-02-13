
add_executable(tps ./src/main.c)
target_link_libraries(tps utils upper_s lower_s rot13_s dl)
target_include_directories(tps PRIVATE  
					../libraries/utils/inc 
					../libraries/upper/inc
					../libraries/lower/inc
					../libraries/rot13/inc
)

add_executable(server ./src/server.c)
target_link_libraries(server utils upper_s lower_s rot13_s dl)
target_include_directories(server PRIVATE  ./inc 
					../libraries/upper/inc
					../libraries/lower/inc
					../libraries/rot13/inc
					../libraries/utils/inc
)

