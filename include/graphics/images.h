#pragma once

#include <unordered_map>

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
	/// @param p PNG file to load into the Vulkan image
	Image( Device& d, Png& p );
	Image( Device& d, VkExtent2D e, VkFormat f );
	~Image();

	Image( Image&& o );
	Image& operator=( Image&& o );

	void transition( const VkImageLayout l );
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
	ImageView( const Image& i );
	~ImageView();

	ImageView( ImageView&& o );
	ImageView& operator=( ImageView&& o );

	Device& device;
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


/// @brief Image repository
class Images
{
  public:
	Images( Device& d );

	/// @brief Loads an image
	/// @return an image view to that image
	VkImageView load( const char* path );

	/// Map of paths and Vulkan images and image views
	std::unordered_map<const char*, std::pair<Image, ImageView>> images = {};

	Device& device;
};


} // namespace graphics
