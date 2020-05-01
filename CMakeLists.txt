

cmake_minimum_required(VERSION 3.15)
project(kaleidoscope)

set(CMAKE_CXX_STANDARD 17)

# GTest Begin
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(LIBRARIES ${LIBRARIES} " " pthread)
else ()
    message("!!! warning: not using pthread " "(host: " ${CMAKE_SYSTEM_NAME} ")!!!")
endif ()

add_subdirectory(googletest)
# GTest End

# LLVM Begin
# 需要提前build好，该目录为LLVMConfig.cmake所在目录
set(LLVM_DIR lib/build-llvm/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
# LLVM End

# include/source files
include_directories(include)
add_subdirectory(src)
