#pragma once

#include <vulkan/vulkan_core.h>

namespace graphics
{

class Device;
class Png;

class Image
{
  public:
	Image( Device& d, Png& p );
	~Image();

	Image( Image&& o );
	Image& operator=( Image&& o );

	Device& device;
	VkImage handle = VK_NULL_HANDLE;
	VkDeviceMemory memory = VK_NULL_HANDLE;
};

} // namespace graphics
