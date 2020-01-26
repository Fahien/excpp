#pragma once

#include <string>
#include <vector>
#include <png.h>

namespace graphics
{

class Png
{
  public:
	Png( const std::string& path );
	~Png();

	void print_info();

	uint32_t width;
	uint32_t height;

	int bit_depth;
	int color_type;
	int interlace_type;
	int compression_type;
	int filter_method;

	png_byte channels;

	std::vector<png_byte> bytes;
	std::vector<png_byte*> rows;
};

} // namespace graphics
