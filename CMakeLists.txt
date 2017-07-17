project(semtest)

cmake_minimum_required(VERSION 2.4.6)

set(CMAKE_CXX_FLAGS "-std=c++0x -O2")

add_executable(semtest semtest.cpp)
