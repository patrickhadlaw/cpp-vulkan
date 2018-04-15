#include "app.h"

cppvk::App::App()
{

}

void cppvk::App::initialize()
{
	DEBUG_FUNCTION_MESSAGE
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
	cppvk::init::createCommandPool(this);
	cppvk::init::createCommandBuffers(this);
	cppvk::init::createSemaphores(this);
}

bool cppvk::App::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void cppvk::App::mainLoop()
{
	glfwPollEvents();
	drawFrame();
}

void cppvk::App::clean()
{
	DEBUG_FUNCTION_MESSAGE
	vkDestroySemaphore(device, imageAvailible, nullptr);
	vkDestroySemaphore(device, renderFinished, nullptr);
	vkDestroyCommandPool(device, commandPool, nullptr);
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

void cppvk::App::deviceWaitIdle()
{
	vkDeviceWaitIdle(device);
}

void cppvk::App::recreateSwapChain()
{
	vkDeviceWaitIdle(device);

	cppvk::init::createSwapChain(this);
	cppvk::init::createImageViews(this);
	cppvk::init::createRenderPass(this);
	cppvk::init::createGraphicsPipeline(this, "shader/hardcodedtriangle.vert.spv", "shader/hardcodedtriangle.frag.spv");
	cppvk::init::createFramebuffers(this);
	cppvk::init::createCommandBuffers(this);
}

void cppvk::App::drawFrame()
{
	uint32_t imageIndex;
	
	VkResult result = vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailible, VK_NULL_HANDLE, &imageIndex);
	if (result != VK_SUCCESS) {
		std::cout << "Failed to aquire next image: " << result << std::endl;
	}
	
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailible };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinished };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		cppvk::error("failed to submit drawing commands", __FILE__, FUNC, __LINE__);
	}
	
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	
	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;
	
	vkQueuePresentKHR(presentQueue, &presentInfo);

	vkQueueWaitIdle(presentQueue);
}
