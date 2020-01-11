#include <cstdlib>
#include <filesystem>
#include <cmath>

#include <graphics/graphics.hpp>


std::vector<graphics::Line> create_square( graphics::Dot a, graphics::Dot b )
{
	std::vector<graphics::Line> lines(4);
	lines[0].a = a;
	lines[0].b.p.x = b.p.x;
	lines[0].b.p.y = a.p.y;

	lines[1].a = lines[0].b;
	lines[1].b = b;

	lines[2].a = b;
	lines[2].b.p.x = a.p.x;
	lines[2].b.p.y = b.p.y;

	lines[3].a = lines[2].b;
	lines[3].b = a;

	return lines;
}


int main()
{
	using namespace graphics;
	auto graphics = Graphics();
	auto square1 = create_square({-0.5, -0.5}, {0.5, 0.5});
	auto square2 = create_square({-0.3, -0.3}, {0.3, 0.3});

	while ( graphics.window.is_alive() )
	{
		graphics.glfw.poll();
		if ( graphics.render_begin() )
		{
			graphics.draw( square1 );
			graphics.draw( square2 );
			graphics.render_end();
		}
	}

	return EXIT_SUCCESS;
}
