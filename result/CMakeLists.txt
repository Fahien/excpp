cmake_minimum_required(VERSION 3.6)
project(Result C CXX)

add_executable(test src/Result.cpp)

target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
