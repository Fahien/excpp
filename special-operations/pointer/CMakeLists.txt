cmake_minimum_required( VERSION 3.7 )

project( pointer LANGUAGES C CXX )

add_executable( pointer-test ${CMAKE_CURRENT_SOURCE_DIR}/Pointer.cpp )
target_link_libraries( pointer-test excppcore )
