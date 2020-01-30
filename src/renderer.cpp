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
: vertex_buffer { d, sizeof( Dot ), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT }
, index_buffer { d, sizeof( Index ), VK_BUFFER_USAGE_INDEX_BUFFER_BIT }
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


void Renderer::add( const Triangle& rect )
{
	// Find Vulkan resources associated to this rect
	auto it = triangle_resources.find( &rect );
	if ( it == std::end( triangle_resources ) )
	{
		auto[new_it, ok] = triangle_resources.emplace(
			&rect,
			Resources( device, swapchain, layout )
		);
		if (ok)
		{
			it = new_it;
		}
	}

	// Vertices
	auto& vertex_buffer = it->second.vertex_buffer;
	vertex_buffer.set_count( rect.dots.size() );
	vertex_buffer.upload( reinterpret_cast<const uint8_t*>( rect.dots.data() ) );

	// Indices
	auto& index_buffer = it->second.index_buffer;
	index_buffer.set_count( rect.indices.size() );
	index_buffer.upload( reinterpret_cast<const uint8_t*>( rect.indices.data() ) );
}


void Renderer::add( const Rect& rect )
{
	// Find Vulkan resources associated to this rect
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

	// Vertices
	auto& vertex_buffer = it->second.vertex_buffer;
	vertex_buffer.set_count( rect.dots.size() );
	vertex_buffer.upload( reinterpret_cast<const uint8_t*>( rect.dots.data() ) );

	// Indices
	auto& index_buffer = it->second.index_buffer;
	index_buffer.set_count( rect.indices.size() );
	index_buffer.upload( reinterpret_cast<const uint8_t*>( rect.indices.data() ) );
}


void Renderer::add( const Mesh& mesh )
{
	// Find Vulkan resources associated to this mesh
	auto it = mesh_resources.find( &mesh );
	if ( it == std::end( mesh_resources ) )
	{
		auto[new_it, ok] = mesh_resources.emplace(
			&mesh,
			Resources( device, swapchain, layout )
		);
		if (ok)
		{
			it = new_it;
		}
	}

	// Vertices
	auto& vertex_buffer = it->second.vertex_buffer;
	vertex_buffer.set_count( mesh.dots.size() );
	vertex_buffer.upload( reinterpret_cast<const uint8_t*>( mesh.dots.data() ) );

	// Indices
	auto& index_buffer = it->second.index_buffer;
	index_buffer.set_count( mesh.indices.size() );
	index_buffer.upload( reinterpret_cast<const uint8_t*>( mesh.indices.data() ) );
}


} // namespace graphics
