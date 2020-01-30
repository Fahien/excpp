#include <cstdlib>
#include <filesystem>
#include <cmath>

#include <graphics/graphics.hpp>
#include <graphics/png.h>

void update( const double dt, graphics::Triangle& r )
{
	r.ubo.model.rotateY( math::radians( dt * 16.0 ) );
}

void update_rect( const double dt, graphics::UniformBufferObject& ubo )
{
	ubo.model.rotateZ( -math::radians( dt * 16.0 ) );
}

void run();

int main()
{
	using namespace graphics;

	//auto image = Png( "img/lena.png" );

	run();

	return EXIT_SUCCESS;
}

graphics::Mesh create_quad()
{
	using namespace graphics;

	Mesh quad;

	quad.dots = {
		Dot( Point( -0.5f, -0.5f ), Color( 0.3f, 0.0f, 0.0f, 0.5f ) ),
		Dot( Point( 0.5f, -0.5f ), Color( 0.0f, 0.3f, 0.0f, 0.5f ) ),
		Dot( Point( -0.5f, 0.5f ), Color( 0.3f, 0.0f, 0.3f, 0.5f ) ),
		Dot( Point( 0.5f, 0.5f ), Color( 0.0f, 0.0f, 0.3f, 0.5f ) )
	};

	quad.indices = { 0, 1, 2, 3, 2, 1 };

	return quad;
}

void run()
{
	using namespace graphics;

	auto graphics = Graphics();

	auto image = Png( "img/lena.png" );
	auto image_size = image.get_size();
	auto staging_buffer = Buffer( graphics.device, image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT );
	auto mem = reinterpret_cast<png_byte*>( staging_buffer.map( image_size ) );
	image.load( mem );
	staging_buffer.unmap();

	auto quad = create_quad();
	auto square = Rect( Dot( Point( -0.5f, -0.5f ) ), Dot( Point( 0.5f, 0.5f ) ) );
	auto triangle = Triangle( Dot( Point( -0.3f, -0.3f ) ), Dot( Point( 0.3f, -0.3f ) ), Dot( Point( 0.0f, 0.3f ) ) );
	graphics.renderer.add( square );
	graphics.renderer.add( triangle );
	graphics.renderer.add( quad );

	while ( graphics.window.is_alive() )
	{
		graphics.glfw.poll();
		auto dt = graphics.glfw.get_delta();

		update(dt, triangle);
		update_rect(dt, square.ubo);
		update_rect(dt, quad.ubo);

		if ( graphics.render_begin() )
		{
			graphics.draw( quad );
			graphics.draw( square );
			graphics.draw( triangle );
			graphics.render_end();
		}
	}
}
