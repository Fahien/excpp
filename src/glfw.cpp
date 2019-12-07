#include <graphics/glfw.hpp>

#include <cassert>

#include <GLFW/glfw3.h>

#include "graphics/graphics.hpp"

namespace graphics
{


Glfw::Glfw()
{
	glfwInit();

	required_extensions.names = glfwGetRequiredInstanceExtensions( &required_extensions.count );

	assert( glfwVulkanSupported() && "Vulkan not supported" );
}


Glfw::~Glfw()
{
	glfwTerminate();
}


void Glfw::poll()
{
	glfwPollEvents();
}


Glfw::Window::Window()
{
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	handle = glfwCreateWindow( 640, 480, "Graphics", nullptr, nullptr );
}


Glfw::Window::~Window()
{
	glfwDestroyWindow( handle );
}


bool Glfw::Window::is_alive()
{
	return !glfwWindowShouldClose( handle );
}


Glfw::Window::Surface::Surface( Instance& i, Glfw::Window& window )
: instance { i }
{
	auto err = glfwCreateWindowSurface( instance.handle, window.handle, nullptr, &handle );

	assert( err == VK_SUCCESS && "Cannot create Surface" );
}


Glfw::Window::Surface::~Surface()
{
	if ( handle != VK_NULL_HANDLE )
	{
		vkDestroySurfaceKHR( instance.handle, handle, nullptr );
	}
}


}
