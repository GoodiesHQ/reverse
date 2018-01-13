cmake_minimum_required(VERSION 3.9)
project(reverse C)

set(CMAKE_C_STANDARD 99)

add_definitions(-DDEBUG)
add_executable(reverse main.c client.c server.c server.h client.h reverse.h reverse.c buffer.c buffer.h)
if(WIN32)
    target_link_libraries(MyExe wsock32 ws2_32)
endif()