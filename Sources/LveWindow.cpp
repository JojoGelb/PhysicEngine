#include "LveWindow.h"

lve::LveWindow::LveWindow(int w, int h, std::string name):width(w),height(h),windowName(name)
{
	initWindow();
}

lve::LveWindow::~LveWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void lve::LveWindow::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//														  fullscreen,for opengl context			
	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

}
