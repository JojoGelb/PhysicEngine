#include "Window.h"
#include <stdexcept>

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Window::Window(int _width, int _height, std::string _name):width(_width), height(_height), name(_name)
{
	glfwSetErrorCallback(glfw_error_callback);
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	//														  fullscreen,for opengl context			
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::Update()
{
	glfwPollEvents();
}

void Window::Shutdown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

VkExtent2D Window::GetExtent()
{
	return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

void Window::CreateWindowSurface(VkInstance& instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to craete window surface");
	}
}

void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto lveWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	lveWindow->framebufferResized = true;
	lveWindow->width = width;
	lveWindow->height = height;
}

