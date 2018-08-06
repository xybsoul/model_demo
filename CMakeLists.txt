cmake_minimum_required(VERSION 3.6)
project(model_demo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_subdirectory(util)
add_subdirectory(common)
add_executable(model_demo main.cpp)

target_link_libraries(model_demo Util Matrix ${Boost_LIBRARIES})