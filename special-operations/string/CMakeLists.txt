cmake_minimum_required( VERSION 3.7 )

project( iterator LANGUAGES C CXX )

add_executable( iterator-test 
	${CMAKE_CURRENT_SOURCE_DIR}/Iterator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Test.cpp
)
target_include_directories( iterator-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries( iterator-test excppcore )
