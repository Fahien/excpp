#pragma once

#include <vulkan/vulkan_core.h>

#include "graphics/commands.h"


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

	VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
	CommandPool command_pool;
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


class Sampler
{
  public:
	Sampler( Device& d );
	~Sampler();

	Sampler( Sampler&& o );
	Sampler& operator=( Sampler&& o );

	Device& device;
	VkSampler handle = VK_NULL_HANDLE;
};


} // namespace graphics
