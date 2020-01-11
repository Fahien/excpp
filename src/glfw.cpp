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

	time = glfwGetTime();

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


double Glfw::get_delta()
{
	auto current = glfwGetTime();
	auto delta = current - time;
	time = current;
	return delta;
}


void set_window_size( GLFWwindow* handle, const int width, const int height )
{
	auto window = reinterpret_cast<Glfw::Window*>( glfwGetWindowUserPointer( handle ) );
	window->extent.width  = width;
	window->extent.height = height;
}


Glfw::Window::Window()
{
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	handle = glfwCreateWindow( extent.width, extent.height, "Graphics", nullptr, nullptr );

	// Resize callback
	glfwSetWindowUserPointer( handle, this );
	glfwSetWindowSizeCallback( handle, set_window_size );
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
