#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace graphics
{

class Device;


class Buffer
{
  public:
	Buffer( Device& d, VkDeviceSize size, VkBufferUsageFlags usage );
	~Buffer();

	Buffer( Buffer&& o );
	Buffer& operator=( Buffer&& o );

	void upload( const uint8_t* data, VkDeviceSize size );

	Device& device;
	VkBuffer handle = VK_NULL_HANDLE;
	VkDeviceMemory memory = VK_NULL_HANDLE;
};


class DynamicBuffer
{
  public:
	/// @param size Size of each vertex buffer
	DynamicBuffer( Device& d, VkDeviceSize size, VkBufferUsageFlags usage, uint32_t capacity = 4 );
	~DynamicBuffer();

	DynamicBuffer( DynamicBuffer&& o );
	DynamicBuffer& operator=( DynamicBuffer&& o );

	uint32_t count() const { return element_count; }

	/// @param count Number of elements in the buffer
	void set_count( uint32_t count );

	/// @param data Pointer to a single element
	/// @param index Element index
	void upload( const uint8_t* data, uint32_t index );

	/// @param data Pointer to a collection of vertices
	void upload( const uint8_t* data );

	Device& device;

	/// Size of a single buffer
	VkDeviceSize size = 0;
	VkBufferUsageFlags usage = 0;
	VkBuffer handle = VK_NULL_HANDLE;
	VkDeviceSize offset = 0;
	VkDeviceMemory memory = VK_NULL_HANDLE;

  private:
	void clear();
	void create_buffers( uint32_t count );

	/// Current number of elements in the buffer
	uint32_t element_count = 0;
};


} // namespace
