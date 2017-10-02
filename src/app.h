#pragma once

#include "shaderprogram.h"

namespace cppvk {

	enum AppFlags {
		USE_IMMEDIATE_SWAP = FLAG_1,
		USE_VSYNC_SWAP = FLAG_2,
		USE_RELAXED_VSYNC_SWAP = FLAG_3,
		USE_TRIPLE_BUFFER_SWAP = FLAG_4
	};

	class App {
		friend init;
		friend QueueFamilyIndex;
		friend SwapChainDetails;
	public:
		App();

		const int WIDTH = 800;
		const int HEIGHT = 600;

		void initialize(); // Initialize glfw window and vulkan
		void mainLoop();
		bool shouldClose();
		void clean(); // Garbage collection for all data

	private:
		GLFWwindow* window;
		VkInstance instance;
		VkDevice device;
		VkDebugReportCallbackEXT callback;
		VkSurfaceKHR surface;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkSwapchainKHR swapChain;

		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;

		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkCommandPool commandPool;

		uint16_t flags = USE_VSYNC_SWAP;
	};
}