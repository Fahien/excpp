#include "graphics/renderer.hpp"

#include "graphics/graphics.hpp"


namespace graphics
{


Renderer::Renderer( Device& d, Swapchain& s, PipelineLayout& l )
: device { d }
, swapchain { s }
, layout { l }
{}


Resources::Resources( Device& d, Swapchain& s, PipelineLayout& l )
: vertex_buffer { d, sizeof( Dot ) }
, uniform_buffers {}
, descriptor_pool { d, static_cast<uint32_t>( s.images.size() ) }
, descriptor_sets { descriptor_pool.allocate( l.descriptor_set_layout, static_cast<uint32_t>( s.images.size() ) ) }
{
	for ( size_t i = 0; i < s.images.size(); ++i )
	{
		uniform_buffers.emplace_back(
			Buffer( d, sizeof( UniformBufferObject ), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT )
		);

		VkDescriptorBufferInfo buffer_info = {};
		buffer_info.buffer = uniform_buffers[i].handle;
		buffer_info.offset = 0;
		buffer_info.range = sizeof( UniformBufferObject );

		VkWriteDescriptorSet descriptor_write = {};
		descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor_write.dstSet = descriptor_sets[i];
		descriptor_write.dstBinding = 0;
		descriptor_write.dstArrayElement = 0;

		descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptor_write.descriptorCount = 1;

		descriptor_write.pBufferInfo = &buffer_info;

		vkUpdateDescriptorSets(d.handle, 1, &descriptor_write, 0, nullptr);
	}
}


void Renderer::add( const Rect& rect )
{
	std::vector<const Dot*> dots = {
		&rect.a, &rect.b,
		&rect.b, &rect.c,
		&rect.c, &rect.d,
		&rect.d, &rect.a
	};

	// Upload dots
	auto it = rect_resources.find( &rect );
	if ( it == std::end( rect_resources ) )
	{
		auto[new_it, ok] = rect_resources.emplace(
			&rect,
			Resources( device, swapchain, layout )
		);
		if (ok)
		{
			it = new_it;
		}
	}

	uint32_t vertex_count = 8;
	auto& vertex_buffer = it->second.vertex_buffer;
	vertex_buffer.set_vertex_count( 8 );
	for (uint32_t i = 0; i < vertex_count; ++i )
	{
		vertex_buffer.upload( reinterpret_cast<const uint8_t*>( dots[i] ), i );
	}
}

} // namespace graphics
