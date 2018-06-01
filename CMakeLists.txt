cmake_minimum_required(VERSION 3.9)
project(lab4)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(test)


add_executable(lab4 server.cpp)

target_link_libraries(lab4)

#add_definitions(-DTESTS)
