#include "graphics/buffers.h"

#include <cassert>
#include <cstring>

#include "graphics/graphics.hpp"

namespace graphics
{


Buffer::Buffer( Device& d, const VkDeviceSize size, const VkBufferUsageFlags usage )
: device { d }
{
	VkBufferCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.size = size;
	info.usage = usage;

	// Not share between multiple queues at the same time
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	auto res = vkCreateBuffer( device.handle, &info, nullptr, &handle );
	assert( res == VK_SUCCESS && "Cannot create buffer" );

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements( device.handle, handle, &requirements );

	VkMemoryAllocateInfo meminfo = {};
	meminfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	meminfo.allocationSize = requirements.size;
	auto memory_type = device.physical_device.get_memory_type( requirements.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
	meminfo.memoryTypeIndex = memory_type;

	res = vkAllocateMemory( device.handle, &meminfo, nullptr, &memory );
	assert( res == VK_SUCCESS && "Cannot allocate memory" );

	res = vkBindBufferMemory( device.handle, handle, memory, /* offset */ 0 );
	assert( res == VK_SUCCESS && "Cannot bind memory to buffer" );
}


Buffer::~Buffer()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyBuffer( device.handle, handle, nullptr );
	}

	if ( memory != VK_NULL_HANDLE )
	{
		vkFreeMemory( device.handle, memory, nullptr );
	}
}


Buffer::Buffer( Buffer&& o )
: device { o.device }
, handle { o.handle }
, memory { o.memory }
{
	o.handle = VK_NULL_HANDLE;
	o.memory = VK_NULL_HANDLE;
}


Buffer& Buffer::operator=( Buffer&& o )
{
	assert( device.handle == o.device.handle && "Cannot move assign buffer from different device" );
	std::swap( handle, o.handle );
	std::swap( memory, o.memory );

	return *this;
}


void* Buffer::map( const VkDeviceSize size )
{
	void* mem;
	vkMapMemory( device.handle, memory, 0, size, 0, &mem);
	return mem;
}


void Buffer::unmap()
{
	vkUnmapMemory( device.handle, memory );
}


void Buffer::upload( const uint8_t* data, const VkDeviceSize size )
{
	void* temp = map( size );
	std::memcpy( temp, data, size );
	unmap();
}


void DynamicBuffer::create_buffers( const uint32_t count )
{
	assert( count > 0 && "Cannot create 0 buffers" );

	clear();

	VkBufferCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.size = count * size;
	info.usage = usage;
	// Not share between multiple queues at the same time
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	auto res = vkCreateBuffer( device.handle, &info, nullptr, &handle );
	assert( res == VK_SUCCESS && "Cannot create buffer" );

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements( device.handle, handle, &requirements );

	VkMemoryAllocateInfo meminfo = {};
	meminfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	// Allocate enough memory for each buffer
	meminfo.allocationSize = requirements.size * count;
	auto memory_type = device.physical_device.get_memory_type( requirements.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
	meminfo.memoryTypeIndex = memory_type;

	if ( memory != VK_NULL_HANDLE )
	{
		// Release previous memory
		vkFreeMemory( device.handle, memory, nullptr );
	}

	res = vkAllocateMemory( device.handle, &meminfo, nullptr, &memory );
	assert( res == VK_SUCCESS && "Cannot allocate memory" );

	res = vkBindBufferMemory( device.handle, handle, memory, 0 );
	assert( res == VK_SUCCESS && "Cannot bind memory to buffer" );
}

DynamicBuffer::DynamicBuffer( Device& d, const VkDeviceSize ds, const VkBufferUsageFlags u, const uint32_t count )
: device { d }
, size { ds }
, usage { u }
{
	create_buffers( count );
}

void DynamicBuffer::clear()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyBuffer( device.handle, handle, nullptr );
		handle = VK_NULL_HANDLE;
	}

	if ( memory != VK_NULL_HANDLE )
	{
		vkFreeMemory( device.handle, memory, nullptr );
		memory = VK_NULL_HANDLE;
	}
}


DynamicBuffer::~DynamicBuffer()
{
	clear();
}


DynamicBuffer::DynamicBuffer( DynamicBuffer&& other )
: device { other.device }
, element_count { other.element_count }
, size { other.size }
, usage { other.usage }
, handle { other.handle }
, offset { other.offset }
, memory { other.memory }
{
	other.handle = VK_NULL_HANDLE;
	other.memory = VK_NULL_HANDLE;
}


DynamicBuffer& DynamicBuffer::operator=( DynamicBuffer&& other )
{
	assert( device.handle == other.device.handle && "Cannot move assign vertex buffers from different devices" );
	std::swap( element_count, other.element_count );
	std::swap( size, other.size );
	std::swap( usage, other.usage );
	std::swap( handle, other.handle );
	std::swap( offset, other.offset );
	std::swap( memory, other.memory );

	return *this;
}


void DynamicBuffer::set_count( const uint32_t count )
{
	create_buffers( count );
	element_count = count;
}


void DynamicBuffer::upload( const uint8_t* data, const uint32_t index )
{
	assert( index < element_count && "Cannot upload vertex out of bounds" );
	void* temp;
	VkDeviceSize offset = size * index;
	vkMapMemory( device.handle, memory, offset, size, 0, &temp);
	std::memcpy( temp, data, size );
	vkUnmapMemory( device.handle, memory );
}


void DynamicBuffer::upload( const uint8_t* data )
{
	void* temp;
	vkMapMemory( device.handle, memory, 0, size * element_count, 0, &temp);
	std::memcpy( temp, data, size * element_count );
	vkUnmapMemory( device.handle, memory );
}


} // namespace graphics
