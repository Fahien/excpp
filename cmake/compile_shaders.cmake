find_program(GLSLV glslangValidator REQUIRED)
add_custom_target(shaders
	COMMAND
		${GLSLV}
			-V ${CMAKE_CURRENT_SOURCE_DIR}/test/shader/triangle.vert
			-o ${CMAKE_CURRENT_BINARY_DIR}/vert.spv
	COMMAND
		${GLSLV}
		-V ${CMAKE_CURRENT_SOURCE_DIR}/test/shader/triangle.frag
		-o ${CMAKE_CURRENT_BINARY_DIR}/frag.spv
)
