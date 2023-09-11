#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>


namespace lve {
	class LveWindow{

	public:
		LveWindow(int w, int h, std::string name);
		~LveWindow();

		//Prevents copy pointer to be created
		//that would result in dangling pointer if windows
		// is deleted
		LveWindow(const LveWindow&) = delete;
		LveWindow& operator=(const LveWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
		
	private:
		GLFWwindow* window;
		std::string windowName;

		const int width;
		const int height;


		
		void initWindow();

		
	};
}



