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


class VertexBuffers
{
  public:
	/// @param size Size of each vertex buffer
	VertexBuffers( Device& d, VkDeviceSize size, uint32_t capacity = 4 );
	~VertexBuffers();

	VertexBuffers( VertexBuffers&& o );
	VertexBuffers& operator=( VertexBuffers&& o );

	uint32_t get_vertex_count() const { return vertex_count; }
	/// @param count Number of vertices to draw
	void set_vertex_count( uint32_t count );

	/// @param data Pointer to a single vertex
	/// @param index Vertex buffer index
	void upload( const uint8_t* data, uint32_t index );

	/// @param data Pointer to a collection of vertices
	void upload( const uint8_t* data );

	Device& device;

	/// Size of a single vertex buffer
	VkDeviceSize size = 0;
	std::vector<VkBuffer> handles;
	std::vector<VkDeviceSize> offsets;
	VkDeviceMemory memory = VK_NULL_HANDLE;

  private:
	void clear();
	void create_buffers( uint32_t count );

	/// Current number of vertices
	/// It can be less than the size of handles
	uint32_t vertex_count = 0;
};


} // namespace
