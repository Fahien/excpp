#include <cstdlib>
#include <filesystem>

#include <graphics/graphics.hpp>


int main()
{
	using namespace graphics;
	auto graphics = Graphics();
	size_t lines_count = 64;
	std::vector<Line> lines(lines_count);
	std::vector<Point> points(2);
	
	for ( size_t i = 0; i < lines_count; ++i )
	{
		lines[i].a.x = -1.0f + i%2 * 2.0f;
		lines[i].a.y = -1.0f;

		lines[i].b.x = -1.0f + 2 * (i+i%2*1) / float(lines_count);
		lines[i].b.y = 1.0;
	}

	points[0].x = -0.125f;
	points[1].x = 0.125f;
	points[0].y = points[1].y = -0.750f;

	while ( graphics.window.is_alive() )
	{
		graphics.glfw.poll();
		if ( graphics.render_begin() )
		{
			graphics.draw( lines );
			graphics.draw( points );
			graphics.render_end();
		}
	}

	return EXIT_SUCCESS;
}
