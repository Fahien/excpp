#include "graphics/descriptors.h"

#include <cassert>

#include "graphics/graphics.hpp"

namespace graphics
{


DescriptorSetLayout::DescriptorSetLayout( Device& d , const std::vector<VkDescriptorSetLayoutBinding>& bindings )
: device { d }
{
	VkDescriptorSetLayoutCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.bindingCount = bindings.size();
	info.pBindings = bindings.data();

	auto res = vkCreateDescriptorSetLayout( device.handle, &info, nullptr, &handle );
	assert( res == VK_SUCCESS && "Cannot create descriptor set layout" );
}


DescriptorSetLayout::~DescriptorSetLayout()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyDescriptorSetLayout( device.handle, handle, nullptr );
	}
}


DescriptorSetLayout::DescriptorSetLayout( DescriptorSetLayout&& o )
: device { o.device }
, handle { o.handle }
{
	o.handle = VK_NULL_HANDLE;
}

DescriptorSetLayout& DescriptorSetLayout::operator=( DescriptorSetLayout&& o )
{
	assert( device.handle == o.device.handle && "Cannot move assign descriptor set layout from different devices" );
	std::swap( handle, o.handle );
	return *this;
}


DescriptorPool::DescriptorPool( Device& d,
	const std::vector<VkDescriptorPoolSize>& pool_sizes,
	const uint32_t sz )
: device { d }
, size { sz }
{
	VkDescriptorPoolCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.poolSizeCount = pool_sizes.size();
	info.pPoolSizes = pool_sizes.data();
	info.maxSets = size;

	auto res = vkCreateDescriptorPool( device.handle, &info, nullptr, &handle );
	assert( res == VK_SUCCESS && "Cannot create descriptor pool" );
}


DescriptorPool::~DescriptorPool()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyDescriptorPool( device.handle, handle, nullptr );
	}
}


std::vector<VkDescriptorSet> DescriptorPool::allocate( const DescriptorSetLayout& layout, const uint32_t count )
{
	const std::vector<VkDescriptorSetLayout> layouts( count, layout.handle );

	VkDescriptorSetAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	info.descriptorPool = handle;
	info.descriptorSetCount = count;
	info.pSetLayouts = layouts.data();

	std::vector<VkDescriptorSet> sets( count );
	const auto res = vkAllocateDescriptorSets( device.handle, &info, sets.data() );
	assert( res == VK_SUCCESS && "Cannot create descriptor sets" );

	return sets;
}

DescriptorPool::DescriptorPool( DescriptorPool&& o )
: device { o.device }
, size { o.size }
, handle { o.handle }
{
	o.handle = VK_NULL_HANDLE;
}

DescriptorPool& DescriptorPool::operator=( DescriptorPool&& o )
{
	assert( device.handle == o.device.handle && "Cannot move assign descriptor set layout from different devices" );
	std::swap( size, o.size );
	std::swap( handle, o.handle );
	return *this;
}



} // namespace graphics
