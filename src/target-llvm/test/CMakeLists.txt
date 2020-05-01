
cmake_minimum_required(VERSION 3.15)
project(kaleidoscope)

add_gtest(target_llvm_test)
target_link_libraries(target_llvm_test ${LLVMLIBS})
