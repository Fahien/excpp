#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace graphics
{

class Device;

class DescriptorSetLayout
{
  public:
	DescriptorSetLayout( Device& d, const std::vector<VkDescriptorSetLayoutBinding>& bindings );
	~DescriptorSetLayout();

	DescriptorSetLayout( DescriptorSetLayout&& o );
	DescriptorSetLayout& operator=( DescriptorSetLayout&& o );

	Device& device;

	VkDescriptorSetLayout handle = VK_NULL_HANDLE;
};


class DescriptorPool
{
  public:
	/// @param size Size of the pool, number of descriptors available
	DescriptorPool( Device& d,
		const std::vector<VkDescriptorPoolSize>& pool_sizes,
		uint32_t size );
	~DescriptorPool();

	DescriptorPool( DescriptorPool&& o );
	DescriptorPool& operator=( DescriptorPool&& o );

	std::vector<VkDescriptorSet> allocate( const DescriptorSetLayout& layout, uint32_t count = 1 );

	Device& device;
	uint32_t size;

	VkDescriptorPool handle = VK_NULL_HANDLE;
};


} // namespace graphics
