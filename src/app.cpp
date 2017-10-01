#include "app.h"

cppvk::App::App()
{

}

void cppvk::App::initialize()
{
	cppvk::init::glfw(this);

	cppvk::init::createInstance(this);
	cppvk::init::createDebugCallback(this);
	cppvk::init::createSurface(this);
	cppvk::init::pickPhysicalDevice(this);
	cppvk::init::createDevice(this);
	cppvk::init::createSwapChain(this);
}

bool cppvk::App::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void cppvk::App::mainLoop()
{
	glfwPollEvents();
}

void cppvk::App::clean()
{
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroyDevice(device, nullptr);
	cppvk::destroyDebugReportCallbackEXT(instance, callback, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}
