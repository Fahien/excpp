cmake_minimum_required( VERSION 3.6 )
project( Editor C CXX )

set( CMAKE_CXX_STANDARD 17 )

set( INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} )

set( SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Editor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/File.cpp )

add_executable( editor ${SOURCES} )
target_include_directories( editor PUBLIC ${INCLUDES} )
