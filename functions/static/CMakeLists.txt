cmake_minimum_required( VERSION 3.6 )
project( Static C CXX )

set( CMAKE_CXX_STANDARD 17 )

set( INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} )

set( STC_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/TestStatic.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Static.cpp )

add_executable( test-static ${STC_SOURCES} )
target_include_directories( test-static PUBLIC ${INCLUDES} )
