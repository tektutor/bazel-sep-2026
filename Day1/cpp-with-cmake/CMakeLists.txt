cmake_minimum_required(VERSION 3.10)

project(HelloWorld VERSION 10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app src/main.cpp)
