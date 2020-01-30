#pragma once

#include <vulkan/vulkan_core.h>

namespace graphics
{

class Device;
class Png;
class Buffer;

class Image
{
  public:
	Image( Device& d, Png& p );
	~Image();

	Image( Image&& o );
	Image& operator=( Image&& o );

	void upload( Buffer& b );

	Device& device;
	VkExtent3D extent = {};
	VkFormat format = VK_FORMAT_UNDEFINED;

	VkImage handle = VK_NULL_HANDLE;
	VkDeviceMemory memory = VK_NULL_HANDLE;
};


class ImageView
{
  public:
	ImageView( Device& d, Image& i );
	~ImageView();

	ImageView( ImageView&& o );
	ImageView& operator=( ImageView&& o );

	Device& device;
	Image& image;

	VkImageView handle = VK_NULL_HANDLE;
};


} // namespace graphics
