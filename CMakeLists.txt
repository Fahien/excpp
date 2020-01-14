cmake_minimum_required( VERSION 3.10 )
project( graphics )

# Dep
list( APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake )

find_package( Vulkan )
message( STATUS ${Vulkan_INCLUDE_DIRS} )
find_package( GLFW )
include(compile_shaders)

# Lib
set( SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/buffers.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/descriptors.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/graphics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/glfw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/math.cpp
)
add_library( ${PROJECT_NAME} ${SOURCES} )
target_include_directories( ${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${Vulkan_INCLUDE_DIRS} )
target_link_libraries( ${PROJECT_NAME} ${Vulkan_LIBRARIES} ${GLFW_LIBRARY} )
target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_17 )

# Test
set( TEST_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
)
add_executable( ${PROJECT_NAME}-test ${TEST_SOURCES} )
target_link_libraries( ${PROJECT_NAME}-test ${PROJECT_NAME} )
target_compile_features( ${PROJECT_NAME}-test PUBLIC cxx_std_17 )
add_dependencies(${PROJECT_NAME}-test shaders)
