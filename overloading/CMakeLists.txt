cmake_minimum_required( VERSION 3.6 )
project( Overloading C CXX )

set( CMAKE_CXX_STANDARD 17 )

set( INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} )

set( STR_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/ExString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TestString.cpp )

add_executable( test-string ${STR_SOURCES} )
target_include_directories( test-string PUBLIC ${INCLUDES} )

add_subdirectory( integer )
add_subdirectory( complex )
