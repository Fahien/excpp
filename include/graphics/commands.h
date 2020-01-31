#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>


namespace graphics
{

class Device;
class RenderPass;
class Framebuffer;
class Buffer;
class DynamicBuffer;
class GraphicsPipeline;
class PipelineLayout;
class Image;

class CommandBuffer
{
  public:
	CommandBuffer( VkCommandBuffer h = VK_NULL_HANDLE );

	void begin( VkCommandBufferUsageFlags usage_flags = 0 );

	void transition( Image& image, VkImageLayout layout );
	void copy( const Buffer& from, const Image& dest );

	void set_viewport( const VkViewport& vp );
	void set_scissor( const VkRect2D& scissor );

	void begin_render_pass( RenderPass& rp, Framebuffer& fb );

	void bind_vertex_buffer( Buffer& b );
	void bind_vertex_buffers( DynamicBuffer& db );

	void bind_index_buffer( DynamicBuffer& b );

	void bind( GraphicsPipeline& p );

	void bind_descriptor_sets( const PipelineLayout& layout, VkDescriptorSet set );

	void draw( const uint32_t vertex_count = 1 );
	void draw_indexed( const uint32_t index_count );

	void end_render_pass();

	void end();

	VkCommandBuffer handle = VK_NULL_HANDLE;
};


class CommandPool
{
  public:
	CommandPool( Device& d, uint32_t family_index = 0 );
	~CommandPool();

	CommandPool( CommandPool&& o );
	CommandPool& operator=( CommandPool&& o );

	std::vector<CommandBuffer> allocate_command_buffers( uint32_t count = 1 );

	Device& device;
	VkCommandPool handle = VK_NULL_HANDLE;
};


} // namespace graphics
