cmake_minimum_required(VERSION 3.10)
project(task)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/task.cc)
add_executable(${PROJECT_NAME} ${SOURCES})
