#include "graphics/renderer.hpp"

#include "graphics/graphics.hpp"


namespace graphics
{


Renderer::Renderer( Device& d, Swapchain& s, PipelineLayout& ll, PipelineLayout& ml )
: device { d }
, swapchain { s }
, line_layout { ll }
, mesh_layout { ml }
{}


std::vector<VkDescriptorPoolSize> get_uniform_pool_sizes( const uint32_t count )
{
	VkDescriptorPoolSize pool_size = {};
	pool_size.descriptorCount = count;
	pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	return { pool_size };
}


Resources::Resources( Device& d, Swapchain& s, PipelineLayout& l )
: vertex_buffer { d, sizeof( Dot ), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT }
, index_buffer { d, sizeof( Index ), VK_BUFFER_USAGE_INDEX_BUFFER_BIT }
, uniform_buffers {}
, descriptor_pool {
		d,
		get_uniform_pool_sizes( s.images.size() ),
		static_cast<uint32_t>(s.images.size())
	}
, descriptor_sets { descriptor_pool.allocate( l.descriptor_set_layout, s.images.size() ) }
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


std::vector<VkDescriptorPoolSize> get_mesh_pool_size( const uint32_t count )
{
	std::vector<VkDescriptorPoolSize> pool_sizes(2);

	pool_sizes[0].descriptorCount = count;
	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[1].descriptorCount = count;
	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	return pool_sizes;
}


MeshResources::MeshResources( Device& d, Swapchain& s, PipelineLayout& l, ImageView& image_view )
: vertex_buffer { d, sizeof( Vertex ), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT }
, index_buffer { d, sizeof( Index ), VK_BUFFER_USAGE_INDEX_BUFFER_BIT }
, uniform_buffers {}
, sampler { d }
, descriptor_pool {
		d,
		get_mesh_pool_size( s.images.size() ),
		static_cast<uint32_t>(s.images.size())
	}
, descriptor_sets { descriptor_pool.allocate( l.descriptor_set_layout, s.images.size() ) }
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

		VkDescriptorImageInfo image_info = {};
		image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		image_info.imageView = image_view.handle;
		image_info.sampler = sampler.handle;

		std::array<VkWriteDescriptorSet, 2> writes = {};
		writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[0].dstSet = descriptor_sets[i];
		writes[0].dstBinding = 0;
		writes[0].dstArrayElement = 0;
		writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writes[0].descriptorCount = 1;
		writes[0].pBufferInfo = &buffer_info;

		writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[1].dstSet = descriptor_sets[i];
		writes[1].dstBinding = 1;
		writes[1].dstArrayElement = 0;
		writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writes[1].descriptorCount = 1;
		writes[1].pImageInfo = &image_info;

		vkUpdateDescriptorSets( d.handle, writes.size(), writes.data(), 0, nullptr );
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
			Resources( device, swapchain, line_layout )
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
			Resources( device, swapchain, line_layout )
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
			MeshResources( device, swapchain, mesh_layout, *mesh.image_view )
		);
		if (ok)
		{
			it = new_it;
		}
	}

	// Vertices
	auto& vertex_buffer = it->second.vertex_buffer;
	vertex_buffer.set_count( mesh.vertices.size() );
	vertex_buffer.upload( reinterpret_cast<const uint8_t*>( mesh.vertices.data() ) );

	// Indices
	auto& index_buffer = it->second.index_buffer;
	index_buffer.set_count( mesh.indices.size() );
	index_buffer.upload( reinterpret_cast<const uint8_t*>( mesh.indices.data() ) );
}


} // namespace graphics
