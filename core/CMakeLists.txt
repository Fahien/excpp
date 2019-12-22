cmake_minimum_required( VERSION 3.6 )
project( excppcore LANGUAGES C CXX )

set( INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} )

set( STR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ExString.cpp )

add_library( ${PROJECT_NAME} ${STR_SOURCES} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${INCLUDES} )
target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_17 )
