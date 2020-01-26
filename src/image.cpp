#include <graphics/image.h>

#include <cassert>

#include <graphics/png.h>
#include <graphics/graphics.hpp>

namespace graphics
{

VkFormat get_format( Png& png )
{
	if ( png.color_type == PNG_COLOR_TYPE_RGBA )
	{
		if ( png.bit_depth == 8 && png.channels == 4 )
		{
			return VK_FORMAT_R8G8B8A8_UNORM;
		}
	}

	assert( false && "Vulkan format not supported" );

	return VK_FORMAT_UNDEFINED;
}

Image::Image( Device& d, Png& png )
: device { d }
{
	// Image
	VkImageCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	info.imageType = VK_IMAGE_TYPE_2D;
	info.extent.width = png.width;
	info.extent.height = png.height;
	info.extent.depth = 1;
	info.mipLevels = 1;
	info.arrayLayers = 1;
	info.tiling = VK_IMAGE_TILING_OPTIMAL;
	info.format = get_format( png );
	info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.samples = VK_SAMPLE_COUNT_1_BIT;

	auto res = vkCreateImage( device.handle, &info, nullptr, &handle );
	assert( res == VK_SUCCESS && "Cannot create image" );

	// Memory
	{
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements( device.handle, handle, &requirements);

		VkMemoryAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.allocationSize = requirements.size;
		auto memory_type = device.physical_device.get_memory_type(
			requirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		info.memoryTypeIndex = memory_type;

		auto res = vkAllocateMemory( device.handle, &info, nullptr, &memory );
		assert( res == VK_SUCCESS && "Cannot allocate memory for image" );

		vkBindImageMemory( device.handle, handle, memory, 0 );
	}
}


Image::~Image()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkFreeMemory( device.handle, memory, nullptr );
		vkDestroyImage( device.handle, handle, nullptr );
	}
}


Image::Image( Image&& other )
: device { other.device }
, handle { other.handle }
, memory { other.memory }
{
	other.handle = VK_NULL_HANDLE;
	other.memory = VK_NULL_HANDLE;
}


Image& Image::operator=( Image&& other )
{
	assert( device.handle == other.device.handle && "Cannot move images from different device" );
	std::swap( handle, other.handle );
	std::swap( memory, other.memory );

	return *this;
}


} // namespace graphics
