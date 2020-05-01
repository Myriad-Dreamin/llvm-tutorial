cmake_minimum_required(VERSION 3.15)
project(kaleidoscope)

# test_main source files -> compile to lib to reduce compile time
add_library(gtest-main STATIC test/test-main.cpp)
target_link_libraries(gtest-main gtest)
target_include_directories(gtest-main SYSTEM PUBLIC gtest gmock)

# description: create a gtest target
# param:
#     test_name: string, the name of target (also the name of entry file)
# example:
#     if there is a test source file named my_test
#         my_test.cpp (without main function, which provided by test_main.cpp)
#     then add following target to CMakeList.txt
#     add_gtest(my_test)
function(add_gtest test_name)
    add_executable(${test_name} ${ARGN} ${test_name}.cpp)
    target_link_libraries(${test_name} gtest-main)
    if (MINGW)
        target_link_libraries(${test_name} -static-libgcc -static-libstdc++)
    endif ()
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_subdirectory(lexer/test)
add_subdirectory(parser/test)
add_subdirectory(target-llvm)

