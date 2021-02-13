#pragma once

#include <unordered_map>

#include "graphics/buffers.h"
#include "graphics/descriptors.h"

namespace graphics
{

struct Rect;
struct Triangle;
class Device;
class Swapchain;
class PipelineLayout;

struct Resources
{
	Resources( Device& device, Swapchain& swapchain, PipelineLayout& l );

	DynamicBuffer vertex_buffer;
	DynamicBuffer index_buffer;
	std::vector<Buffer> uniform_buffers;
	DescriptorPool descriptor_pool;
	std::vector<VkDescriptorSet> descriptor_sets;
};

class Renderer
{
  public:
	Renderer( Device& d, Swapchain& s, PipelineLayout& l );

	void add( const Triangle& t );
	void add( const Rect& r );

	Device& device;
	Swapchain& swapchain;
	PipelineLayout& layout;

	/// @brief Each model will have
	/// - vertex buffer containing constant data about its vertices
	/// - uniform buffers that can change per swapchain image
	/// - DescriptorPool and DescriptorSet per swapchain image
	std::unordered_map<const Rect*, Resources> rect_resources;
	std::unordered_map<const Triangle*, Resources> triangle_resources;
};

}
