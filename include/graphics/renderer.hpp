#pragma once

#include <unordered_map>

#include "graphics/buffers.h"
#include "graphics/descriptors.h"
#include "graphics/image.h"

namespace graphics
{

struct Rect;
struct Triangle;
struct Mesh;

class Device;
class Swapchain;
class PipelineLayout;

struct Resources
{
	Resources( Device& device, Swapchain& swapchain, PipelineLayout& l );

	// Vertices and indices do not change, hence one is enough
	DynamicBuffer vertex_buffer;
	DynamicBuffer index_buffer;

	// Uniform buffer for each swapchain image
	std::vector<Buffer> uniform_buffers;

	/// Descriptor pool for descriptor sets
	DescriptorPool descriptor_pool;
	// Descriptor sets for each swapchain image
	std::vector<VkDescriptorSet> descriptor_sets;
};


struct MeshResources
{
	MeshResources( Device& device, Swapchain& swapchain, PipelineLayout& l, ImageView& image_view );

	// Vertices and indices do not change, hence one is enough
	DynamicBuffer vertex_buffer;
	DynamicBuffer index_buffer;

	// Uniform buffer for each swapchain image
	std::vector<Buffer> uniform_buffers;

	//ImageView image_view;
	Sampler sampler;

	/// Descriptor pool for descriptor sets
	DescriptorPool descriptor_pool;
	// Descriptor sets for each swapchain image
	std::vector<VkDescriptorSet> descriptor_sets;
};

class Renderer
{
  public:
	Renderer( Device& d, Swapchain& s, PipelineLayout& line_layout, PipelineLayout& mesh_layout );

	void add( const Triangle& t );
	void add( const Rect& r );
	void add( const Mesh& m );

	Device& device;
	Swapchain& swapchain;
	PipelineLayout& line_layout;
	PipelineLayout& mesh_layout;

	/// @brief Each model will have
	/// - vertex buffer containing constant data about its vertices
	/// - uniform buffers that can change per swapchain image
	/// - DescriptorPool and DescriptorSet per swapchain image
	std::unordered_map<const Rect*, Resources> rect_resources;
	std::unordered_map<const Triangle*, Resources> triangle_resources;
	std::unordered_map<const Mesh*, MeshResources> mesh_resources;
};

}
