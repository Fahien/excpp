cmake_minimum_required(VERSION 3.10)
project(charvec)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/charvec.cc)
add_executable(${PROJECT_NAME} ${SOURCES})
