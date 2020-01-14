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


void Buffer::upload( const uint8_t* data, const VkDeviceSize size )
{
	void* temp;
	vkMapMemory( device.handle, memory, 0, size, 0, &temp);
	std::memcpy( temp, data, size );
	vkUnmapMemory( device.handle, memory );
}


void VertexBuffers::create_buffers( const uint32_t count )
{
	assert( count > 0 && "Cannot create 0 buffers" );

	/// TODO: fix this class
	if (count > handles.size())
	{
		clear();

		VkBufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.size = count * size;
		info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		// Not share between multiple queues at the same time
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		// Make space for handles
		handles.resize( 1 );
		offsets.resize( 1 );

		for ( uint32_t i = 0; i < 1; ++i )
		{
			offsets[i] = size * i;
			auto res = vkCreateBuffer( device.handle, &info, nullptr, &handles[i] );
			assert( res == VK_SUCCESS && "Cannot create buffer" );
		}

		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements( device.handle, handles[0], &requirements );

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

		auto res = vkAllocateMemory( device.handle, &meminfo, nullptr, &memory );
		assert( res == VK_SUCCESS && "Cannot allocate memory" );

		res = vkBindBufferMemory( device.handle, handles[0], memory, 0 );
		assert( res == VK_SUCCESS && "Cannot bind memory to buffer" );
	}
}

VertexBuffers::VertexBuffers( Device& d, const VkDeviceSize ds, const uint32_t count )
: device { d }
, size { ds }
{
	create_buffers( count );
}

void VertexBuffers::clear()
{
	for ( auto handle : handles )
	{
		vkDestroyBuffer( device.handle, handle, nullptr );
		handles.clear();
	}

	if ( memory != VK_NULL_HANDLE )
	{
		vkFreeMemory( device.handle, memory, nullptr );
		memory = VK_NULL_HANDLE;
	}
}


VertexBuffers::~VertexBuffers()
{
	clear();
}


VertexBuffers::VertexBuffers( VertexBuffers&& other )
: device { other.device }
, vertex_count { other.vertex_count }
, size { other.size }
, handles { std::move( other.handles ) }
, offsets { std::move( other.offsets ) }
, memory { other.memory }
{
	other.memory = VK_NULL_HANDLE;
}


VertexBuffers& VertexBuffers::operator=( VertexBuffers&& other )
{
	assert( device.handle == other.device.handle && "Cannot move assign vertex buffers from different devices" );
	std::swap( vertex_count, other.vertex_count );
	std::swap( size, other.size );
	std::swap( handles, other.handles );
	std::swap( offsets, other.offsets );
	std::swap( memory, other.memory );

	return *this;
}


void VertexBuffers::set_vertex_count( const uint32_t count )
{
	create_buffers( count );
	vertex_count = count;
}


void VertexBuffers::upload( const uint8_t* data, const uint32_t index )
{
	assert( index < vertex_count && "Cannot upload vertex out of bounds" );
	void* temp;
	VkDeviceSize offset = size * index;
	vkMapMemory( device.handle, memory, offset, size, 0, &temp);
	std::memcpy( temp, data, size );
	vkUnmapMemory( device.handle, memory );
}


void VertexBuffers::upload( const uint8_t* data )
{
	void* temp;
	vkMapMemory( device.handle, memory, 0, size * vertex_count, 0, &temp);
	std::memcpy( temp, data, size * vertex_count );
	vkUnmapMemory( device.handle, memory );
}


} // namespace graphics
