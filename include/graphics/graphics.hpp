#pragma once

#include <fstream>
#include <filesystem>
#include <functional>

#include <vulkan/vulkan_core.h>

#include "graphics/glfw.hpp"
#include "graphics/renderer.hpp"
#include "graphics/descriptors.h"
#include "math/math.h"


namespace graphics
{


struct alignas(16) Color
{
	Color( float rr = 0.0f, float gg = 0.0f, float bb = 0.0f, float aa = 1.0f )
	: r { rr }, g { gg }, b { bb }, a { aa } {}

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;
};


struct alignas(16) Point
{
	Point( float xx = 0.0f, float yy = 0.0f ): x { xx }, y { yy } {}

	float x = 0.0f;
	float y = 0.0f;
};


struct alignas(16) Dot
{
	Dot( float x = 0.0f, float y = 0.0f, Color cc = { 1.0f } ) : p { x, y }, c { cc } {}

	Point p = {};
	Color c = {};
};


struct Line
{
	Line( Dot aa = {}, Dot bb = {} ) : a { aa }, b { bb } {}

	Dot a = {};
	Dot b = {};
};


struct Rect
{
	Rect( Dot bottom_left, Dot top_right );

	Rect( Dot aa, Dot bb, Dot cc, Dot dd )
	: a { aa }, b { bb }, c { cc }, d { dd } {}

	Dot a = {};
	Dot b = {};
	Dot c = {};
	Dot d = {};

	math::Mat4 model = math::Mat4::identity;
};


struct alignas(16) UniformBufferObject
{
	math::Mat4 model;
};


template<typename T>
VkVertexInputBindingDescription get_bindings();

template<typename T>
std::vector<VkVertexInputAttributeDescription> get_attributes();


struct ValidationLayers
{
	uint32_t count = 0;
	const char** names = nullptr;
};


class PhysicalDevice
{
  public:
	PhysicalDevice( VkPhysicalDevice h );

	VkSurfaceCapabilitiesKHR get_capabilities( VkSurfaceKHR s );
	std::vector<VkSurfaceFormatKHR> get_formats( VkSurfaceKHR s );
	std::vector<VkPresentModeKHR> get_present_modes( VkSurfaceKHR s );
	uint32_t get_memory_type( uint32_t type_filter, VkMemoryPropertyFlags f );

	VkPhysicalDevice handle = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties properties;
	std::vector<VkQueueFamilyProperties> queue_families;
	std::vector<VkExtensionProperties> extensions;
};


class Device;
class CommandBuffer;


class Semaphore
{
  public:
	Semaphore( Device& d );
	~Semaphore();

	Semaphore( Semaphore&& other );
	Semaphore& operator=( Semaphore&& other );

	Device& device;
	VkSemaphore handle = VK_NULL_HANDLE;
};


class Fence
{
  public:
	Fence( Device& d );
	~Fence();

	Fence( Fence&& o );
	Fence& operator=( Fence&& o );

	void wait() const;
	void reset();

	Device& device;
	VkFence handle = VK_NULL_HANDLE;
	bool can_wait = true;
};


class Queue
{
  public:
	Queue( const Device& d, uint32_t family_index, uint32_t index );

	bool supports_present( VkSurfaceKHR s );

	void submit( CommandBuffer& command_buffer, const std::vector<VkSemaphore>& waits, const std::vector<VkSemaphore>& signals = {}, Fence* fence = nullptr );

	VkResult present( const std::vector<VkSwapchainKHR>& swapchains, const std::vector<uint32_t>& image_index, const std::vector<VkSemaphore>& waits = {} );

	const Device& device;
	uint32_t family_index = 0;
	uint32_t index = 0;
	VkQueue handle = VK_NULL_HANDLE;
	VkQueueFlags flags = 0;
};


class Swapchain;


class Device
{
  public:
	Device( PhysicalDevice& p, VkSurfaceKHR s, RequiredExtensions required_extensions = {} );
	~Device();

	void wait_idle() const;
	Queue& find_queue( VkQueueFlagBits flags );
	Queue& find_graphics_queue();
	Queue& find_present_queue( VkSurfaceKHR surface );

	PhysicalDevice& physical_device;
	VkSurfaceKHR surface;
	VkDevice handle = VK_NULL_HANDLE;
	std::vector<Queue> queues;
};


class Instance
{
  public:
	Instance( RequiredExtensions req_ext = {}, ValidationLayers layers = {} );
	~Instance();

	VkInstance handle = VK_NULL_HANDLE;
	std::vector<PhysicalDevice> physical_devices;
};


class Swapchain;


class RenderPass
{
  public:
	RenderPass( Swapchain& s );
	~RenderPass();

	RenderPass& operator=( RenderPass&& o );

	Device& device;
	VkRenderPass handle = VK_NULL_HANDLE;
};


class Framebuffer
{
  public:
	Framebuffer( VkImageView view, VkExtent2D& extent, RenderPass& render_pass );
	~Framebuffer();

	Framebuffer( Framebuffer&& other );

	Device& device;
	VkFramebuffer handle = VK_NULL_HANDLE;

	VkExtent2D extent;
};


class Swapchain
{
  public:
	Swapchain( Device& d );
	~Swapchain();

	std::vector<Framebuffer> create_framebuffers( RenderPass& render_pass );

	void recreate();

	Device& device;
	VkSwapchainKHR handle = VK_NULL_HANDLE;

	VkFormat format;
	VkExtent2D extent;

	std::vector<VkImage> images;
	std::vector<VkImageView> views;

  private:
	void create();

	void destroy_views();
};

class GraphicsPipeline;
class PipelineLayout;

class CommandBuffer
{
  public:
	CommandBuffer( VkCommandBuffer h = VK_NULL_HANDLE );

	void begin();

	void set_viewport( const VkViewport& vp );
	void set_scissor( const VkRect2D& scissor );

	void begin_render_pass( RenderPass& rp, Framebuffer& fb );

	void bind_vertex_buffer( Buffer& b );
	void bind_vertex_buffers( VertexBuffers& vbs );

	void bind( GraphicsPipeline& p );

	void bind_descriptor_sets( const PipelineLayout& layout, VkDescriptorSet set );

	void draw( const uint32_t vertex_count = 1 );

	void end_render_pass();

	void end();

	VkCommandBuffer handle = VK_NULL_HANDLE;
};


class CommandPool
{
  public:
	CommandPool( Device& d, uint32_t family_index = 0 );
	~CommandPool();

	std::vector<CommandBuffer> allocate_command_buffers( uint32_t count = 1 );

	Device& device;
	VkCommandPool handle = VK_NULL_HANDLE;
};


class ShaderModule
{
  public:
	ShaderModule( Device& d, const std::filesystem::path& path );
	~ShaderModule();

	ShaderModule( ShaderModule&& );
	ShaderModule& operator=( ShaderModule&& );

	Device& device;
	VkShaderModule handle = VK_NULL_HANDLE;
};


class PipelineLayout
{
  public:
	PipelineLayout( Device& d );
	~PipelineLayout();

	Device& device;
	DescriptorSetLayout descriptor_set_layout;

	VkPipelineLayout handle = VK_NULL_HANDLE;
};


class GraphicsPipeline
{
  public:
	GraphicsPipeline(
		PipelineLayout& layout,
		ShaderModule& vert,
		ShaderModule& frag,
		RenderPass& render_pass,
		const VkViewport& viewport,
		const VkRect2D& scissor,
		VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST );
	~GraphicsPipeline();

	GraphicsPipeline& operator=( GraphicsPipeline&& o );

	Device& device;
	VkPipeline handle = VK_NULL_HANDLE;
};


class Graphics
{
  public:
	Graphics();

	bool render_begin();
	void render_end();

	void draw();
	void draw( const std::vector<const Dot*>& dots );
	void draw( const std::vector<Dot>& dots );
	void draw( const std::vector<Line>& lines );
	void draw( const Rect& rect );

	Glfw glfw;
	Instance instance;
	Glfw::Window window;
	Glfw::Window::Surface surface;
	const char* swapchain_extension_name = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	RequiredExtensions device_required_extensions { 1, &swapchain_extension_name };
	Device device;
	Swapchain swapchain;

	RenderPass render_pass;

	ShaderModule vert;
	ShaderModule frag;
	PipelineLayout layout;

	VkViewport viewport = {};
	VkRect2D   scissor  = {};

	GraphicsPipeline line_pipeline;
	GraphicsPipeline dot_pipeline;

	Renderer renderer;

	CommandPool command_pool;
	std::vector<CommandBuffer> command_buffers;
	CommandBuffer* current_command_buffer = nullptr;

	std::vector<Framebuffer> framebuffers;
	Framebuffer* current_framebuffer = nullptr;

	uint32_t current_frame_index = 0;

	std::vector<Semaphore> images_available;
	Semaphore* current_image_available = nullptr;

	std::vector<Semaphore> images_drawn;
	std::vector<Fence> frames_in_flight;
	Fence* current_frame_in_flight = nullptr;

	Queue& graphics_queue;
	Queue& present_queue;
};


} // namespace graphics
