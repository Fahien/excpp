cmake_minimum_required( VERSION 3.6 )

project( integer LANGUAGES C CXX )

add_executable( integer-test ${CMAKE_CURRENT_SOURCE_DIR}/Integer.cpp )
