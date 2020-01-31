#include "graphics/png.h"

#include <cassert>
#include <png.h>
#include <cstdio>
#include <cstdlib>

#include "graphics/graphics.hpp"

namespace graphics
{


void handle_error( png_struct* png, const char* msg )
{
	std::fprintf( stderr, "PNG error: %s\n", msg );
	exit( EXIT_FAILURE );
}


void handle_warning( png_struct* png, const char* msg )
{
	std::fprintf( stderr, "PNG warning: %s\n", msg );
	exit( EXIT_FAILURE );
}


void read_status( png_struct* png, uint32_t row, int pass )
{
	Png* obj = ( Png* )png_get_error_ptr( png );
	
	uint32_t percent = row * 100.0f / obj->height;
	std::fprintf( stdout, "\rRead\t%u%%", percent );
	std::fflush( stdout );
}


const char* color_type_to_string( int color_type )
{
	switch ( color_type )
	{
		case PNG_COLOR_TYPE_GA: return "Gray Alpha";
		case PNG_COLOR_TYPE_GRAY: return "Gray";
		case PNG_COLOR_TYPE_PALETTE: return "Palette";
		case PNG_COLOR_TYPE_RGB: return "RGB";
		case PNG_COLOR_TYPE_RGBA: return "RGBA";
		default: return "UNKNOWN";
	}
}

void Png::print_info()
{
	std::printf( "Size\t%ux%u\n", width, height );
	std::printf( "Depth\t%i\n", bit_depth );
	std::printf( "Color\t%s\n", color_type_to_string( color_type ) );
	std::printf( "Chans\t%d\n", channels );
}


Png::Png( const std::string& path )
{
	png = png_create_read_struct( PNG_LIBPNG_VER_STRING, this, handle_error, handle_warning );
	assert( png && "Cannot create PNG read" );

	info = png_create_info_struct( png );
	assert( info && "Cannot create PNG info" );
	
	end = png_create_info_struct( png );
	assert( end && "Cannot create PNG end info" );

	auto res = setjmp( png_jmpbuf( png ) );
	assert( !res && "Cannot set PNG jump" );

	file = std::fopen( path.c_str(), "rb" );
	assert( file && "Cannot open png file" );

	png_init_io( png, file );

	// Meter
	png_set_read_status_fn( png, read_status );

	png_read_info( png, info );

	png_get_IHDR( png, info, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method );

	if ( color_type == PNG_COLOR_TYPE_RGB )
	{
		png_set_add_alpha( png, 255, PNG_FILLER_AFTER );
		png_read_update_info( png, info );
		color_type = png_get_color_type( png, info );
	}

	assert( bit_depth == 8 && "PNG bit depth not supported" );
	channels = png_get_channels( png, info );

	print_info();
}


size_t Png::get_size() const
{
	return width * height * channels;
}


void Png::load( png_byte* bytes )
{
	// Allocate memory to store bytes of the image
	size_t byte_count = get_size();

	// Allocate memory to store pointers to rows
	rows.resize( height );
	for( uint32_t i = 0; i < height; ++i )
	{
		size_t offset = width * channels * i;
		rows[i] = bytes + offset;
	}

	png_read_image( png, rows.data() );
	png_read_end( png, info );
	std::printf("\n");

}

Png::~Png()
{
	png_destroy_read_struct( &png, &info, &end );
	std::fclose( file );
}

} // namespace graphics
