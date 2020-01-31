find_program(GLSLV glslangValidator REQUIRED)
add_custom_target(shaders
	COMMAND
		${GLSLV}
			-V ${CMAKE_CURRENT_SOURCE_DIR}/test/shader/line.vert
			-o ${CMAKE_CURRENT_BINARY_DIR}/line.vert.spv
	COMMAND
		${GLSLV}
			-V ${CMAKE_CURRENT_SOURCE_DIR}/test/shader/line.frag
			-o ${CMAKE_CURRENT_BINARY_DIR}/line.frag.spv
	COMMAND
		${GLSLV}
			-V ${CMAKE_CURRENT_SOURCE_DIR}/test/shader/mesh.vert
			-o ${CMAKE_CURRENT_BINARY_DIR}/mesh.vert.spv
	COMMAND
		${GLSLV}
			-V ${CMAKE_CURRENT_SOURCE_DIR}/test/shader/mesh.frag
			-o ${CMAKE_CURRENT_BINARY_DIR}/mesh.frag.spv
)
