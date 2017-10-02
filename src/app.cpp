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
	cppvk::init::createImageViews(this);
	cppvk::init::createRenderPass(this);
	cppvk::init::createGraphicsPipeline(this, "shader/hardcodedtriangle.vert.spv", "shader/hardcodedtriangle.frag.spv");
	cppvk::init::createFramebuffers(this);
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
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
	}
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		vkDestroyImageView(device, swapChainImageViews[i], nullptr);
	}
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroyDevice(device, nullptr);
	cppvk::destroyDebugReportCallbackEXT(instance, callback, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}
