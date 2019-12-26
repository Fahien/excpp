#include <cstdlib>
#include <filesystem>

#include <graphics/graphics.hpp>


int main()
{
	using namespace graphics;
	auto graphics = Graphics();
	
	while ( graphics.window.is_alive() )
	{
		graphics.glfw.poll();
		if ( graphics.render_begin() )
		{
			//graphics.draw();
			graphics.draw( Point( -0.5f, -0.5f ) );
			graphics.render_end();
		}
	}

	return EXIT_SUCCESS;
}