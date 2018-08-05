cmake_minimum_required(VERSION 3.6)
project(model_demo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_subdirectory(util)
add_subdirectory(common)
add_executable(model_demo main.cpp util/random.h)

target_link_libraries(model_demo Util Matrix)