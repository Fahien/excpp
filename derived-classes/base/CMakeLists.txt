cmake_minimum_required( VERSION 3.10 )

project( base )

set( SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Test.cpp
)

add_executable( ${PROJECT_NAME} ${SOURCES} )
