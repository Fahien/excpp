#include <cstdlib>
#include <filesystem>

#include <graphics/graphics.hpp>


int main()
{
	auto graphics = graphics::Graphics();
	
	while ( graphics.window.is_alive() )
	{
		graphics.glfw.poll();
		graphics.draw();
	}

	return EXIT_SUCCESS;
}