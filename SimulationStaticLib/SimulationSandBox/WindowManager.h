#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
class WindowManager
{

	GLFWwindow* _window{ nullptr };
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	bool framebufferResized = false;



public:
	WindowManager() = default;
	~WindowManager() = default;

	void initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(_window, this);
		glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto wm = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
		if (wm) {
			wm->framebufferResized = true;
		}
	}

	void setWindowShouldClose(bool shouldClose)
	{
		if (shouldClose)
		{
			glfwSetWindowShouldClose(_window, GLFW_TRUE);
		}
		else
		{
			glfwSetWindowShouldClose(_window, GLFW_FALSE);
		}
	}

	bool getShouldWindowClose() const
	{
		return glfwWindowShouldClose(_window);
	}

	void destroyWindow() {
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	GLFWwindow* getWindow() const { return _window; }

	uint32_t getWidth() const { return WIDTH; }
	uint32_t getHeight() const { return HEIGHT; }

	bool isFramebufferResized() const { return framebufferResized; }
	void resetFramebufferResized() { framebufferResized = false; }

};

