project(test)

add_subdirectory(gtest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests ../client_proto.cpp ../client_proto.h tests.cpp)

target_link_libraries(runTests gtest gtest_main)
target_link_libraries(runTests)
