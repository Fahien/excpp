#include <cstdlib>
#include <filesystem>
#include <cmath>

#include <graphics/graphics.hpp>


void update( const double dt, graphics::Rect& r )
{
	r.model.rotateZ( math::radians( dt * 16.0 ) );
}

void update2( const double dt, graphics::Rect& r )
{
	r.model.rotateZ( -math::radians( dt * 16.0 ) );
}

int main()
{
	using namespace graphics;
	auto graphics = Graphics();
	auto square1 = Rect({-0.5, -0.5}, {0.5, 0.5});
	auto square2 = Rect({-0.3, -0.3}, {0.3, 0.3});
	graphics.renderer.add( square1 );
	graphics.renderer.add( square2 );

	while ( graphics.window.is_alive() )
	{
		graphics.glfw.poll();
		auto dt = graphics.glfw.get_delta();

		update(dt, square2);
		update2(dt, square1);

		if ( graphics.render_begin() )
		{
			graphics.draw( square1 );
			graphics.draw( square2 );
			graphics.render_end();
		}
	}

	return EXIT_SUCCESS;
}
