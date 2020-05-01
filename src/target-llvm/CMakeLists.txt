
cmake_minimum_required(VERSION 3.15)
project(kaleidoscope)

llvm_map_components_to_libnames(LLVMLIBS support core irreader)

add_executable(kale-llvm compiler.cpp)
add_subdirectory(test)
target_link_libraries(kale-llvm ${LLVMLIBS})
