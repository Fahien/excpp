#pragma once

#include <string>
#include <vector>
#include <png.h>

namespace graphics
{

class Device;

class Png
{
  public:
	Png( const std::string& path );
	~Png();

	void print_info();

	size_t get_size() const;

	void load( png_byte* bytes );

	png_struct* png = nullptr;
	png_info* info = nullptr;
	png_info* end = nullptr;

	FILE* file = nullptr;

	uint32_t width;
	uint32_t height;

	int bit_depth;
	int color_type;
	int interlace_type;
	int compression_type;
	int filter_method;

	png_byte channels;

	std::vector<png_byte*> rows;
};

} // namespace graphics
