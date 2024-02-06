cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

project("Dog class")

add_executable(run src/main.cpp headers/My_shared_ptr.hpp)

target_include_directories(run PUBLIC src headers)