cmake_minimum_required(VERSION 2.8)
include(CMakeToolsHelpers OPTIONAL)
project(ExCpp C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)

function(add_exercise CHAPTER EXERCISE)
	set(EXECUTABLE ${CHAPTER}/${EXERCISE}.cpp)
	add_executable(${EXERCISE} ${EXECUTABLE})
	set_target_properties(${EXERCISE} PROPERTIES LINKER_LANGUAGE CXX)
endfunction(add_exercise)

add_exercise(statements Forwhile)
add_exercise(statements Atoi)
add_exercise(statements Itoa)
add_exercise(statements StripComments)

add_exercise(expressions ReadPairs)
add_exercise(expressions Bitwise)
add_exercise(expressions Strings)
add_exercise(expressions Calculator)

add_exercise(select-operations BadAlloc)
add_exercise(select-operations SumElements)
add_exercise(select-operations Apply)
