#pragma once


#include <vector>
#include <string>

#include <vulkan/vulkan_core.h>

struct GLFWwindow;

namespace graphics
{


struct RequiredExtensions
{
	uint32_t count = 0;
	const char** names = nullptr;
};

class Instance;

class Glfw
{
  public:
	Glfw();
	~Glfw();

	void poll();

	RequiredExtensions required_extensions;

	class Window
	{
	  public:
		Window();
		~Window();

		bool is_alive();
		
		GLFWwindow* handle = nullptr;

		VkExtent2D extent = { 600, 600 };

		class Surface
		{
		  public:
			Surface( Instance& i, Window& w );
			~Surface();

			Instance& instance;
			VkSurfaceKHR handle = VK_NULL_HANDLE;
		};
	};
};


}
