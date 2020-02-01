#include "graphics/graphics.hpp"

#include <array>
#include <filesystem>
#include <limits>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <cmath>

namespace graphics
{


Point::Point( float xx, float yy, float zz ) : x { xx }, y { yy }, z { zz } {}


Rect::Rect( Dot bottom_left, Dot top_right )
: dots {
		bottom_left,
		{ { top_right.p.x, bottom_left.p.y, 0.0f }, bottom_left.c },
		top_right,
		{ { bottom_left.p.x, top_right.p.y, 0.0f }, top_right.c }
	}
{
}


template <typename T>
VkVertexInputBindingDescription get_bindings()
{
	VkVertexInputBindingDescription bindings = {};
	bindings.binding = 0;
	bindings.stride = sizeof( T );
	bindings.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindings;
}


template <>
std::vector<VkVertexInputAttributeDescription> get_attributes<Dot>()
{
	std::vector<VkVertexInputAttributeDescription> attributes( 2 );

	attributes[0].binding = 0;
	attributes[0].location = 0;
	attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributes[0].offset = offsetof( Dot, p );

	attributes[1].binding = 0;
	attributes[1].location = 1;
	attributes[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributes[1].offset = offsetof( Dot, c );

	return attributes;
}

template <>
std::vector<VkVertexInputAttributeDescription> get_attributes<Vertex>()
{
	std::vector<VkVertexInputAttributeDescription> attributes( 3 );

	attributes[0].binding = 0;
	attributes[0].location = 0;
	attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributes[0].offset = offsetof( Vertex, p );

	attributes[1].binding = 0;
	attributes[1].location = 1;
	attributes[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributes[1].offset = offsetof( Vertex, c );

	attributes[2].binding = 0;
	attributes[2].location = 2;
	attributes[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributes[2].offset = offsetof( Vertex, t );

	return attributes;
}


PhysicalDevice::PhysicalDevice( VkPhysicalDevice h )
: handle { h }
{
	vkGetPhysicalDeviceProperties( handle, &properties );

	uint32_t queue_family_count;
	vkGetPhysicalDeviceQueueFamilyProperties( handle, &queue_family_count, nullptr );
	queue_families.resize( queue_family_count );
	vkGetPhysicalDeviceQueueFamilyProperties( handle, &queue_family_count, queue_families.data() );

	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties( handle, nullptr, &extension_count, nullptr );
	extensions.resize( extension_count );
	vkEnumerateDeviceExtensionProperties( handle, nullptr, &extension_count, extensions.data() );
}


VkSurfaceCapabilitiesKHR PhysicalDevice::get_capabilities( const VkSurfaceKHR s )
{
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR( handle, s, &capabilities );
	return capabilities;
}


std::vector<VkSurfaceFormatKHR> PhysicalDevice::get_formats( VkSurfaceKHR s )
{
	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR( handle, s, &format_count, nullptr);
	std::vector<VkSurfaceFormatKHR> formats( format_count );
	vkGetPhysicalDeviceSurfaceFormatsKHR( handle, s, &format_count, formats.data() );
	return formats;
}


std::vector<VkPresentModeKHR> PhysicalDevice::get_present_modes( VkSurfaceKHR s )
{
	uint32_t mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR( handle, s, &mode_count, nullptr );
	std::vector<VkPresentModeKHR> modes( mode_count );
	vkGetPhysicalDeviceSurfacePresentModesKHR( handle, s, &mode_count, modes.data() );
	return modes;
}


uint32_t PhysicalDevice::get_memory_type( uint32_t type_filter, VkMemoryPropertyFlags flags )
{
	VkPhysicalDeviceMemoryProperties properties;
	vkGetPhysicalDeviceMemoryProperties( handle, &properties );

	for ( uint32_t i = 0; i < properties.memoryTypeCount; ++i )
	{
		if ( ( type_filter & ( 1 << i ) ) &&
		     ( ( properties.memoryTypes[i].propertyFlags & flags ) == flags ) )
		{
			return i;
		}
	}

	assert( false && "Cannot find memory type" );
	return 0;
}


VkFormatProperties PhysicalDevice::get_format_properties( const VkFormat format )
{
	VkFormatProperties props;
	vkGetPhysicalDeviceFormatProperties( handle, format, &props );
	return props;
}


Queue::Queue( const Device& d, const uint32_t family_index, const uint32_t index )
: device { d }
, family_index { family_index }
, index { index }
, flags { d.physical_device.queue_families[family_index].queueFlags }
{
	vkGetDeviceQueue( d.handle, family_index, index, &handle );
}


bool Queue::supports_present( VkSurfaceKHR s )
{
	VkBool32 support_present;
	vkGetPhysicalDeviceSurfaceSupportKHR( device.physical_device.handle, family_index, s, &support_present );
	return support_present == VK_TRUE;
}


void Queue::submit( CommandBuffer& command_buffer, const std::vector<VkSemaphore>& waits, const std::vector<VkSemaphore>& signals, Fence* fence )
{
	VkSubmitInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	info.waitSemaphoreCount = waits.size();
	info.pWaitSemaphores = waits.data();

	VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	info.pWaitDstStageMask = wait_stages;

	info.commandBufferCount = 1;
	info.pCommandBuffers = &command_buffer.handle;

	info.signalSemaphoreCount = signals.size();
	info.pSignalSemaphores = signals.data();

	auto fence_handle = fence ? fence->handle : VK_NULL_HANDLE;

	auto ret = vkQueueSubmit( handle, 1, &info, fence_handle );
	assert( ret == VK_SUCCESS && "Cannot submit to queue" );

	fence->can_wait = true;
}


VkResult Queue::present( const std::vector<VkSwapchainKHR>& swapchains, const std::vector<uint32_t>& image_indices, const std::vector<VkSemaphore>& waits )
{
	VkPresentInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = waits.size();
	info.pWaitSemaphores = waits.data();

	info.swapchainCount = swapchains.size();
	info.pSwapchains = swapchains.data();
	info.pImageIndices = image_indices.data();

	return vkQueuePresentKHR( handle, &info );
}

Device::Device( PhysicalDevice& d, const VkSurfaceKHR s, const RequiredExtensions required_extensions )
: physical_device { d }
, surface { s }
{
	VkDeviceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	// Queue infos
	VkDeviceQueueCreateInfo queue_info = {};
	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_info.queueFamilyIndex = 0;
	queue_info.queueCount = 1;
	float queue_priority = 1.0f;
	queue_info.pQueuePriorities = &queue_priority;

	info.queueCreateInfoCount = 1;
	info.pQueueCreateInfos = &queue_info;

	// Features
	VkPhysicalDeviceFeatures features = {};
	features.samplerAnisotropy = VK_TRUE;

	info.pEnabledFeatures = &features;

	// Extensions
	if ( required_extensions.count > 0 )
	{
		info.enabledExtensionCount = required_extensions.count;
		info.ppEnabledExtensionNames = required_extensions.names;
	}

	auto ret = vkCreateDevice( d.handle, &info, nullptr, &handle );
	assert( ret == VK_SUCCESS && "Cannot create device" );

	// Get graphics queue
	queues.emplace_back( *this, 0, 0 );
	assert( queues[0].handle != VK_NULL_HANDLE && "Cannot get graphics queue" );
	assert( queues[0].flags & VK_QUEUE_GRAPHICS_BIT && "First queue is not for graphics" );
	assert( queues[0].supports_present( surface ) && "First queue does not support present" );
}


Device::~Device()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyDevice( handle, nullptr );
	}
}


Queue& Device::find_queue( VkQueueFlagBits flags )
{
	auto it = std::find_if( std::begin( queues ), std::end( queues ),
		[flags]( auto& queue ) { return queue.flags & flags; });
	assert( it != std::end( queues ) && "Cannot find graphics queue" );
	return *it;
}


void Device::wait_idle() const
{
	auto res = vkDeviceWaitIdle( handle );
	assert( res == VK_SUCCESS && "Cannot wait for device to be idle" );
}


Queue& Device::find_graphics_queue()
{
	return find_queue( VK_QUEUE_GRAPHICS_BIT );
}


Queue& Device::find_present_queue( VkSurfaceKHR surface )
{
	auto it = std::find_if( std::begin( queues ), std::end( queues ),
		[surface]( graphics::Queue& queue ) { return queue.supports_present( surface ); });
	assert( it != std::end( queues ) && "Cannot find graphics queue" );
	return *it;
}


Instance::Instance( RequiredExtensions req_ext, ValidationLayers layers )
{
	VkInstanceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.enabledExtensionCount = req_ext.count;
	info.ppEnabledExtensionNames = req_ext.names;
	info.enabledLayerCount = layers.count;
	info.ppEnabledLayerNames = layers.names;
	auto res = vkCreateInstance( &info, nullptr, &handle );
	assert( res == VK_SUCCESS && "Cannot create Vulkan instance" );

	uint32_t ph_dev_count;
	vkEnumeratePhysicalDevices( handle, &ph_dev_count, nullptr );

	std::vector<VkPhysicalDevice> ph_devs( ph_dev_count );
	vkEnumeratePhysicalDevices( handle, &ph_dev_count, ph_devs.data() );

	for ( auto& ph_dev : ph_devs )
	{
		physical_devices.emplace_back( ph_dev );
	}
}


Instance::~Instance()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyInstance( handle, nullptr );
	}
}


Semaphore::Semaphore( Device& d )
: device { d }
{
	VkSemaphoreCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	auto ret = vkCreateSemaphore( device.handle, &info, nullptr, &handle );
	assert( ret == VK_SUCCESS && "Cannot create sempahore" );
}


Semaphore::~Semaphore()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroySemaphore( device.handle, handle, nullptr );
	}
}


Semaphore::Semaphore( Semaphore&& other )
: device { other.device }
, handle { other.handle }
{
	other.handle = VK_NULL_HANDLE;
}


Semaphore& Semaphore::operator=( Semaphore&& other )
{
	assert( &device == &other.device && "Cannot move semaphore from different device" );
	std::swap(handle, other.handle);
	return *this;
}


Fence::Fence( Device& d )
: device { d }
{
	VkFenceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	auto res = vkCreateFence( device.handle, &info, nullptr, &handle );
	assert( res == VK_SUCCESS && "Cannot create fence" );
}


Fence::~Fence()
{
	if ( handle != VK_NULL_HANDLE )
	{
		wait();
		vkDestroyFence( device.handle, handle, nullptr );
	}
}


Fence::Fence( Fence&& other )
: device { other.device }
, handle { other.handle }
{
	other.handle = VK_NULL_HANDLE;
}


Fence& Fence::operator=( Fence&& other )
{
	assert( device.handle == other.device.handle
		&& "Cannot move assign fence of another device" );
	std::swap( handle, other.handle );
	return *this;
}


void Fence::wait() const
{
	if ( can_wait )
	{
		auto res = vkWaitForFences( device.handle, 1, &handle, VK_TRUE, std::numeric_limits<uint64_t>::max() );
		assert( res == VK_SUCCESS && "Cannot wait for fence" );
	}
}


void Fence::reset()
{
	auto res = vkResetFences( device.handle, 1, &handle );
	assert( res == VK_SUCCESS && "Cannot reset fence" );
	can_wait = false;
}


VkSurfaceFormatKHR choose_format( const std::vector<VkSurfaceFormatKHR>& formats )
{
	assert( formats.size() > 0 && "No surface formats available" );

	for ( auto& format : formats )
	{
		if ( format.format == VK_FORMAT_B8G8R8A8_UNORM &&
		     format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
		{
			return format;
		}
	}

	return formats[0];
}


VkExtent2D choose_extent( const VkSurfaceCapabilitiesKHR& capabilities )
{
	if ( capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() )
	{
	//	return capabilities.currentExtent;
		return capabilities.minImageExtent;
	}

	return capabilities.minImageExtent;
}


VkPresentModeKHR choose_present_mode( const std::vector<VkPresentModeKHR>& present_modes)
{
	for ( auto mode : present_modes )
	{
		if ( mode == VK_PRESENT_MODE_MAILBOX_KHR )
		{
			return mode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}


Swapchain::Swapchain( Device& d )
: device { d }
{
	create();
}


void Swapchain::create()
{
	VkSwapchainCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.surface = device.surface;

	auto capabilities = device.physical_device.get_capabilities( device.surface );
	info.minImageCount = 3;
	assert( capabilities.maxImageCount == 0 || capabilities.maxImageCount > 2 &&
		"Triple buffering is not supported");
	if ( capabilities.maxImageCount > 0 && info.minImageCount > capabilities.maxImageCount )
	{
		info.minImageCount = capabilities.maxImageCount;
	}

	auto formats = device.physical_device.get_formats( device.surface );
	auto surface_format = choose_format( formats );
	format = surface_format.format;
	info.imageFormat = surface_format.format;
	info.imageColorSpace = surface_format.colorSpace;

	extent = choose_extent( capabilities );
	info.imageExtent = extent;
	info.imageArrayLayers = 1;
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

	info.preTransform = capabilities.currentTransform;

	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	auto present_modes = device.physical_device.get_present_modes( device.surface );
	info.presentMode = choose_present_mode( present_modes );
	info.clipped = VK_TRUE;

	info.oldSwapchain = handle;

	auto ret = vkCreateSwapchainKHR( device.handle, &info, nullptr, &handle );
	assert( ret == VK_SUCCESS && "Cannot create swapchain" );

	uint32_t image_count;
	vkGetSwapchainImagesKHR( device.handle, handle, &image_count, nullptr );

	assert( images.size() == 0 && "Images should not be there" );
	images.resize( image_count );
	vkGetSwapchainImagesKHR( device.handle, handle, &image_count, images.data() );

	assert( views.size() == 0 && "Views should not be there" );
	views.resize( image_count );
	for ( size_t i = 0; i < image_count; ++i )
	{
		VkImageViewCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = images[i];
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.format = format;
		info.components.r = info.components.g = info.components.b = info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		
		info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		info.subresourceRange.levelCount = 1;
		info.subresourceRange.layerCount = 1;

		auto ret = vkCreateImageView( device.handle, &info, nullptr, views.data() + i );
		assert( ret == VK_SUCCESS && "Cannote create swapchain image view" );
	}
}

void Swapchain::destroy_views()
{
	for ( auto view : views )
	{
		vkDestroyImageView( device.handle, view, nullptr );
	}
	views.clear();
	images.clear();
}


Swapchain::~Swapchain()
{
	destroy_views();

	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroySwapchainKHR( device.handle, handle, nullptr );
	}
}


Frames::Frames( Swapchain& swapchain )
{
	for ( size_t i = 0; i < swapchain.images.size(); ++i )
	{
		color_images.emplace_back( swapchain.images[i] );
		color_views.emplace_back( swapchain.views[i] );

		auto depth = Image( swapchain.device, swapchain.extent, VK_FORMAT_D32_SFLOAT );
		depth.transition( VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL );
		auto view = ImageView( depth );
		depth_images.emplace_back( std::move( depth ) );
		depth_views.emplace_back( std::move( view ) );
	}
}


std::vector<Framebuffer> Frames::create_framebuffers( RenderPass& render_pass )
{
	std::vector<Framebuffer> framebuffers;
	for ( size_t i = 0; i < color_views.size(); ++i )
	{
		auto color_view = color_views[i];
		auto depth_view = depth_views[i].handle;
		std::vector<VkImageView> views = { color_view, depth_view };
		VkExtent2D extent = { depth_images[i].extent.width, depth_images[i].extent.height };
		auto framebuffer = Framebuffer( views, extent, render_pass );
		framebuffers.emplace_back( std::move( framebuffer ) );
	}
	return framebuffers;
}


void Swapchain::recreate()
{
	device.wait_idle();
	destroy_views();
	create();
}


ShaderModule::ShaderModule( Device& d, const std::filesystem::path& path )
: device { d }
{
	VkShaderModuleCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.codeSize = std::filesystem::file_size(path);

	std::vector<char> code( info.codeSize );
	std::ifstream( path ).read( code.data(), info.codeSize );
	info.pCode = reinterpret_cast<uint32_t*>( code.data() );

	vkCreateShaderModule(device.handle, &info, nullptr, &handle);
}


RenderPass::RenderPass( Swapchain& s )
: device { s.device }
{
	VkAttachmentDescription color_attachment = {};
	color_attachment.format = s.format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depth_attachment = {};
	depth_attachment.format = VK_FORMAT_D32_SFLOAT;
	depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference color_ref = {};
	color_ref.attachment = 0;
	color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_ref = {};
	depth_ref.attachment = 1;
	depth_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_ref;
	subpass.pDepthStencilAttachment = &depth_ref;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;

	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;

	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = {
		color_attachment, depth_attachment
	};

	VkRenderPassCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = attachments.size();
	info.pAttachments = attachments.data();
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	info.dependencyCount = 1;
	info.pDependencies = &dependency;

	auto ret = vkCreateRenderPass( device.handle, &info, nullptr, &handle );
	assert( ret == VK_SUCCESS && "Cannot create render pass" );
}


RenderPass::~RenderPass()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyRenderPass( device.handle, handle, nullptr );
	}
}


RenderPass& RenderPass::operator=( RenderPass&& other )
{
	assert( device.handle == other.device.handle &&
		"Cannot move assign render pass of another device" );
	std::swap( handle, other.handle );
	return *this;
}


Framebuffer::Framebuffer( const std::vector<VkImageView>& views, VkExtent2D& ext, RenderPass& render_pass )
: device { render_pass.device }
, extent { ext }
{
	VkFramebufferCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	info.renderPass = render_pass.handle;
	info.attachmentCount = views.size();
	info.pAttachments = views.data();
	info.width = extent.width;
	info.height = extent.height;
	info.layers = 1;

	auto ret = vkCreateFramebuffer( device.handle, &info, nullptr, &handle );
	assert( ret == VK_SUCCESS && "Cannot create framebuffer" );
}


Framebuffer::~Framebuffer()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyFramebuffer( device.handle, handle, nullptr );
	}
}


Framebuffer::Framebuffer( Framebuffer&& other )
: device { other.device }
, handle { other.handle }
, extent { other.extent }
{
	other.handle = VK_NULL_HANDLE;
}


ShaderModule::~ShaderModule()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyShaderModule( device.handle, handle, nullptr );
	}
}


ShaderModule::ShaderModule( ShaderModule&& other )
: device { other.device }
, handle { other.handle }
{
	other.handle = VK_NULL_HANDLE;
}


ShaderModule& ShaderModule::operator=( ShaderModule&& other )
{
	assert( &device == &other.device );
	std::swap( handle, other.handle );
	return *this;
}


PipelineLayout::PipelineLayout( Device& d, const std::vector<VkDescriptorSetLayoutBinding>& bindings )
: device { d }
, descriptor_set_layout { d, std::move( bindings ) }
{
	VkPipelineLayoutCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.setLayoutCount = 1;
	info.pSetLayouts = &descriptor_set_layout.handle;

	vkCreatePipelineLayout( device.handle, &info, nullptr, &handle );
}

PipelineLayout::~PipelineLayout()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyPipelineLayout( device.handle, handle, nullptr );
	}
}


GraphicsPipeline::GraphicsPipeline(
	VkVertexInputBindingDescription bindings,
	const std::vector<VkVertexInputAttributeDescription>& attributes,
	PipelineLayout& layout,
	ShaderModule& vert,
	ShaderModule& frag,
	RenderPass& render_pass,
	const VkViewport& viewport,
	const VkRect2D& scissor,
	const VkPrimitiveTopology topology )
: device { vert.device }
{
	VkPipelineVertexInputStateCreateInfo input_info = {};
	input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	input_info.vertexBindingDescriptionCount = 1;
	input_info.pVertexBindingDescriptions = &bindings;
	input_info.vertexAttributeDescriptionCount = attributes.size();
	input_info.pVertexAttributeDescriptions = attributes.data();

	VkPipelineInputAssemblyStateCreateInfo assembly_info = {};
	assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assembly_info.topology = topology;
	assembly_info.primitiveRestartEnable = VK_FALSE;

	std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages = {};

	auto& vert_info = shader_stages[0];
	vert_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vert_info.module = vert.handle;
	vert_info.pName = "main";

	auto& frag_info = shader_stages[1];
	frag_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_info.module = frag.handle;
	frag_info.pName = "main";

	VkPipelineViewportStateCreateInfo viewport_info = {};
	viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_info.viewportCount = 1;
	viewport_info.pViewports = &viewport;
	viewport_info.scissorCount = 1;
	viewport_info.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer_info = {};
	rasterizer_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer_info.depthClampEnable = VK_FALSE;
	rasterizer_info.rasterizerDiscardEnable = VK_FALSE;
	rasterizer_info.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer_info.lineWidth = 1.0f;
	rasterizer_info.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer_info.depthBiasEnable = VK_FALSE;
	rasterizer_info.depthBiasConstantFactor = 0.0f;
	rasterizer_info.depthBiasClamp = 0.0f;
	rasterizer_info.depthBiasSlopeFactor = 0.0f;

	VkPipelineDepthStencilStateCreateInfo depth_info = {};
	depth_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_info.depthTestEnable = VK_TRUE;
	depth_info.depthWriteEnable = VK_TRUE;
	depth_info.depthCompareOp = VK_COMPARE_OP_LESS;
	depth_info.depthBoundsTestEnable = VK_FALSE;
	depth_info.stencilTestEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling_info = {};
	multisampling_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling_info.sampleShadingEnable = VK_FALSE;
	multisampling_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling_info.minSampleShading = 1.0f; // Optional
	multisampling_info.pSampleMask = nullptr; // Optional
	multisampling_info.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling_info.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineColorBlendAttachmentState color_blend_attachment = {};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;
	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo color_blending = {};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY; // Optional
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;
	color_blending.blendConstants[0] = 0.0f; // Optional
	color_blending.blendConstants[1] = 0.0f; // Optional
	color_blending.blendConstants[2] = 0.0f; // Optional
	color_blending.blendConstants[3] = 0.0f; // Optional

	VkDynamicState dynamic_states[] = {
	    VK_DYNAMIC_STATE_VIEWPORT,
	    VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamic_state = {};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = 2;
	dynamic_state.pDynamicStates = dynamic_states;

	VkGraphicsPipelineCreateInfo pipeline_info = {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = shader_stages.size();
	pipeline_info.pStages = shader_stages.data();

	pipeline_info.pVertexInputState = &input_info;
	pipeline_info.pInputAssemblyState = &assembly_info;
	pipeline_info.pViewportState = &viewport_info;
	pipeline_info.pRasterizationState = &rasterizer_info;
	pipeline_info.pMultisampleState = &multisampling_info;
	pipeline_info.pDepthStencilState = &depth_info;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = nullptr; // Optional

	pipeline_info.layout = layout.handle;

	pipeline_info.renderPass = render_pass.handle;
	pipeline_info.subpass = 0;

	auto ret = vkCreateGraphicsPipelines( device.handle, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &handle );
	assert( ret == VK_SUCCESS && "Cannote create graphics line_pipeline" );
}


GraphicsPipeline::~GraphicsPipeline()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroyPipeline( device.handle, handle, nullptr );
	}
}


GraphicsPipeline& GraphicsPipeline::operator=( GraphicsPipeline&& other )
{
	assert( device.handle == other.device.handle &&
		"Cannot move assign graphics line_pipeline of another device" );
	std::swap( handle, other.handle );
	return *this;
}


ValidationLayers get_validation_layers()
{
	static std::vector<const char*> layer_names = {
		"VK_LAYER_KHRONOS_validation"
	};

	ValidationLayers layers = {};
	layers.names = layer_names.data();
	layers.count = layer_names.size();

	return layers;
}


VkViewport create_viewport( const Glfw::Window& window )
{
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = window.extent.width;
	viewport.height = window.extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	return viewport;
}


VkRect2D create_scissor( const Glfw::Window& window )
{
	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = window.extent;
	return scissor;
}


std::vector<VkDescriptorSetLayoutBinding> get_line_bindings()
{
	VkDescriptorSetLayoutBinding binding = {};
	binding.binding = 0;
	binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	binding.descriptorCount = 1;
	binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	return { binding };
}


std::vector<VkDescriptorSetLayoutBinding> get_mesh_bindings()
{
	VkDescriptorSetLayoutBinding ubo_binding = {};
	ubo_binding.binding = 0;
	ubo_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ubo_binding.descriptorCount = 1;
	ubo_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding sampler_binding = {};
	sampler_binding.binding = 1;
	sampler_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	sampler_binding.descriptorCount = 1;
	sampler_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	return { ubo_binding, sampler_binding };
}


math::Mat4 perspective( const float a, const float y, const float f, const float n )
{
	math::Mat4 proj = {};

	// Calculate projection matrix
	float cotfov = 1.0f / std::tan( 0.5f * y );
	proj[0] = cotfov / a;
	proj[5] = cotfov;
	proj[10] = -( n + f ) / ( f - n );
	proj[14] = -2.0f * n * f / ( f - n );
	proj[11] = -1.0f;

	return proj;
}


math::Mat4 look_at( const math::Vec3& eye, const math::Vec3& center, math::Vec3 up )
{
	math::Vec3 forward = eye - center;
	forward.normalize();

	math::Vec3 right = math::Vec3::cross( up, forward );
	right.normalize();

	up = math::Vec3::cross( forward, right );
	up.normalize();

	math::Mat4 matrix = {};

	matrix[0] = right.x;
	matrix[4] = right.y;
	matrix[8] = right.z;
	matrix[12] = -math::Vec3::dot( right, eye );
	matrix[1] = up.x;
	matrix[5] = up.y;
	matrix[9] = up.z;
	matrix[13] = -math::Vec3::dot( up, eye );
	matrix[2] = forward.x;
	matrix[6] = forward.y;
	matrix[10] = forward.z;
	matrix[14] = -math::Vec3::dot( forward, eye );
	matrix[3] = 0;
	matrix[7] = 0;
	matrix[11] = 0;
	matrix[15] = 1.0f;

	return matrix;
}

math::Mat4 ortho( float left, float right, float bottom, float top, float near, float far )
{
	math::Vec3 mid;
	mid.x = ( left + right ) / ( right - left );
	mid.y = ( bottom + top ) / ( bottom - top );
	mid.z = near / ( near - far );

	math::Vec3 scale;
	scale.x = 2.0f / ( right - left );
	scale.y = 2.0f / ( bottom - top );
	scale.z = 1.0f / ( near - far );

	math::Mat4 mat = math::Mat4::identity;

	mat[12] = -mid.x;
	mat[13] = -mid.y;
	mat[14] = mid.z;

	mat[0] = scale.x;
	mat[5] = -scale.y;
	mat[10] = scale.z;

	return mat;
}


Graphics::Graphics()
: instance { glfw.required_extensions, get_validation_layers() }
, surface { instance, window }
, device { instance.physical_devices.at( 0 ), surface.handle, device_required_extensions }
, swapchain { device }
, frames { swapchain }
, render_pass { swapchain }
, line_vert { device, "line.vert.spv" }
, line_frag { device, "line.frag.spv" }
, line_layout { device, get_line_bindings() }
, mesh_vert { device, "mesh.vert.spv" }
, mesh_frag { device, "mesh.frag.spv" }
, mesh_layout { device, get_mesh_bindings() }
, viewport { create_viewport( window ) }
, scissor { create_scissor( window ) }
, line_pipeline {
	get_bindings<Dot>(),
	get_attributes<Dot>(),
	line_layout,
	line_vert,
	line_frag,
	render_pass,
	viewport,
	scissor,
	VK_PRIMITIVE_TOPOLOGY_LINE_LIST }
, dot_pipeline {
	get_bindings<Dot>(),
	get_attributes<Dot>(),
	line_layout,
	line_vert,
	line_frag,
	render_pass,
	viewport,
	scissor,
	VK_PRIMITIVE_TOPOLOGY_POINT_LIST }
, mesh_pipeline {
	get_bindings<Vertex>(),
	get_attributes<Vertex>(),
	mesh_layout,
	mesh_vert,
	mesh_frag,
	render_pass,
	viewport, scissor }
, renderer { device, swapchain, line_layout, mesh_layout }
, command_pool { device }
, command_buffers { command_pool.allocate_command_buffers( swapchain.images.size() ) }
, framebuffers { frames.create_framebuffers( render_pass ) }
, graphics_queue { device.find_graphics_queue() }
, present_queue { device.find_present_queue( surface.handle ) }
, images { device }
, view { look_at(
	math::Vec3( 0.0f, 2.0f, -2.0f ),
	math::Vec3( 0.0f, 0.0f, 0.0f ),
	math::Vec3( 0.0f, 1.0f, 0.0f ) ) }
, proj { ortho( -1.0f, 1.0f, -1.0f, 1.0f, 0.125f, 8.0f ) }
{
	for ( size_t i = 0; i < swapchain.images.size(); ++i )
	{
		images_available.emplace_back( device );
		images_drawn.emplace_back( device );
		frames_in_flight.emplace_back( device );
	}
}


bool Graphics::render_begin()
{
	std::rotate(std::begin(images_available), ++std::begin(images_available), std::end(images_available));
	current_image_available = &images_available.back();

	uint32_t image_index;
	auto res = vkAcquireNextImageKHR( device.handle,
		swapchain.handle,
		std::numeric_limits<uint64_t>::max(),
		current_image_available->handle,
		VK_NULL_HANDLE,
		&image_index );
	if ( res == VK_ERROR_OUT_OF_DATE_KHR ||
		viewport.width != window.extent.width ||
		viewport.height != window.extent.height )
	{
		// Recreate current semaphore
		images_available.back() = Semaphore( device );

		swapchain.recreate();
		render_pass = RenderPass( swapchain );

		// Update viewport and scissor
		viewport.width = window.extent.width;
		viewport.height = window.extent.height;
		scissor.extent = window.extent;

		dot_pipeline = GraphicsPipeline(
			get_bindings<Dot>(),
			get_attributes<Dot>(),
			line_layout,
			line_vert,
			line_frag,
			render_pass,
			viewport,
			scissor, VK_PRIMITIVE_TOPOLOGY_POINT_LIST );
		line_pipeline = GraphicsPipeline(
			get_bindings<Dot>(),
			get_attributes<Dot>(),
			line_layout,
			line_vert,
			line_frag,
			render_pass,
			viewport,
			scissor, VK_PRIMITIVE_TOPOLOGY_LINE_LIST );
		mesh_pipeline = GraphicsPipeline(
			get_bindings<Vertex>(),
			get_attributes<Vertex>(),
			mesh_layout,
			mesh_vert,
			mesh_frag,
			render_pass,
			viewport,
			scissor );

		frames = Frames( swapchain );
		framebuffers = frames.create_framebuffers( render_pass );

		for ( auto& fence : frames_in_flight )
		{
			fence.can_wait = false;
		}

		return false; // then skip frame
	}
	else
	{
		assert( ( res == VK_SUCCESS || res == VK_SUBOPTIMAL_KHR ) && "Cannot present" );
	}

	current_frame_index = image_index;
	current_frame_in_flight = &frames_in_flight[current_frame_index];
	current_frame_in_flight->wait();
	current_frame_in_flight->reset();

	current_command_buffer = &command_buffers[image_index];
	current_framebuffer = &framebuffers[image_index];

	current_command_buffer->begin();
	current_command_buffer->begin_render_pass( render_pass, *current_framebuffer );

	return true;
}


void Graphics::render_end()
{
	current_command_buffer->end_render_pass();
	current_command_buffer->end();

	auto& image_drawn = images_drawn[current_frame_index];

	graphics_queue.submit( *current_command_buffer, { current_image_available->handle }, { image_drawn.handle }, current_frame_in_flight );

	present_queue.present( { swapchain.handle }, { current_frame_index }, { image_drawn.handle } );
}


void Graphics::draw( Triangle& tri )
{
	auto& resources = renderer.triangle_resources.find( &tri )->second;

	tri.ubo.view = view;
	tri.ubo.proj = proj;

	auto data = reinterpret_cast<const uint8_t*>( &tri.ubo );
	auto& uniform_buffer = resources.uniform_buffers[current_frame_index];
	uniform_buffer.upload( data, sizeof( UniformBufferObject ) );

	current_command_buffer->bind( line_pipeline );
	current_command_buffer->bind_vertex_buffers( resources.vertex_buffer );
	current_command_buffer->bind_index_buffer( resources.index_buffer );

	auto& descriptor_set = resources.descriptor_sets[current_frame_index];
	current_command_buffer->bind_descriptor_sets( line_layout, descriptor_set );
	current_command_buffer->draw_indexed( resources.index_buffer.count() );
}


void Graphics::draw( Rect& rect )
{
	auto& resources = renderer.rect_resources.find( &rect )->second;

	rect.ubo.view = view;
	rect.ubo.proj = proj;

	auto data = reinterpret_cast<const uint8_t*>( &rect.ubo );
	auto& uniform_buffer = resources.uniform_buffers[current_frame_index];
	uniform_buffer.upload( data, sizeof( UniformBufferObject ) );

	current_command_buffer->bind( line_pipeline );
	current_command_buffer->bind_vertex_buffers( resources.vertex_buffer );
	current_command_buffer->bind_index_buffer( resources.index_buffer );

	auto& descriptor_set = resources.descriptor_sets[current_frame_index];
	current_command_buffer->bind_descriptor_sets( line_layout, descriptor_set );
	current_command_buffer->draw_indexed( resources.index_buffer.count() );
}


void Graphics::draw( Mesh& mesh )
{
	auto pair = renderer.mesh_resources.find( &mesh );
	assert( pair != std::end( renderer.mesh_resources ) && "Cannot find resources for a mesh" );

	auto& resources = pair->second;

	mesh.ubo.view = view;
	mesh.ubo.proj = proj;

	auto data = reinterpret_cast<const uint8_t*>( &mesh.ubo );
	auto& uniform_buffer = resources.uniform_buffers[current_frame_index];
	uniform_buffer.upload( data, sizeof( UniformBufferObject ) );

	current_command_buffer->bind( mesh_pipeline );
	current_command_buffer->bind_vertex_buffers( resources.vertex_buffer );
	current_command_buffer->bind_index_buffer( resources.index_buffer );

	auto& descriptor_set = resources.descriptor_sets[current_frame_index];
	current_command_buffer->bind_descriptor_sets( mesh_layout, descriptor_set );
	current_command_buffer->draw_indexed( resources.index_buffer.count() );
}


} // namespace graphics
