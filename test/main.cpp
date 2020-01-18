#include <cstdlib>
#include <filesystem>
#include <cmath>

#include <graphics/graphics.hpp>


void update( const double dt, graphics::Triangle& r )
{
	r.model.rotateY( math::radians( dt * 16.0 ) );
}

void update( const double dt, graphics::Rect& r )
{
	r.model.rotateZ( -math::radians( dt * 16.0 ) );
}

int main()
{
	using namespace graphics;
	auto graphics = Graphics();
	auto square = Rect( Dot{ Point{ -0.5f, -0.5f } }, Dot{ Point{ 0.5f, 0.5f } } );
	auto triangle = Triangle( Dot{ Point{ -0.3f, -0.3f } }, Dot{ Point{ 0.3f, -0.3f } }, Dot{ Point{ 0.0f, 0.3f } } );
	graphics.renderer.add( square );
	graphics.renderer.add( triangle );

	while ( graphics.window.is_alive() )
	{
		graphics.glfw.poll();
		auto dt = graphics.glfw.get_delta();

		update(dt, triangle);
		update(dt, square);

		if ( graphics.render_begin() )
		{
			graphics.draw( square );
			graphics.draw( triangle );
			graphics.render_end();
		}
	}

	return EXIT_SUCCESS;
}
