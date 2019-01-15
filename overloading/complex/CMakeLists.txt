cmake_minimum_required( VERSION 3.6 )

project( complex LANGUAGES C CXX )

add_executable( complex-test ${CMAKE_CURRENT_SOURCE_DIR}/Complex.cpp )
