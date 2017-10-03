#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <glm\glm.hpp>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>
#include <fstream>

#define CPPVK_VERSION_MAJOR 1
#define CPPVK_VERSION_MINOR 0
#define CPPVK_VERSION_PATCH 0

#define VULKAN_API_MAJOR 1
#define VULKAN_API_MINOR 0
#define VULKAN_API_PATCH 61

#ifdef _WIN64

#define FUNC __FUNCTION__

#elif defined(_WIN32)

#define FUNC __FUNCTION__

#elif defined(__APPLE__)

#define FUNC __func__

#elif defined(__linux__)

#define FUNC __func__

#else

#define FUNC "compiler does not support function name"

#endif

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#define HEAVY_DEBUG

#ifdef NDEBUG
#define DEBUG_FUNCTION_MESSAGE
namespace cppvk {
	const bool enableValidationLayers = false;
}
#else

#ifdef HEAVY_DEBUG
#define DEBUG_FUNCTION_MESSAGE std::cout << "[" << FUNC << "]\n";
#else
#define DEBUG_FUNCTION_MESSAGE
#endif

namespace cppvk {
	const bool enableValidationLayers = true;
}
#endif

namespace cppvk {

	enum Flag {
		FLAG_1 = 0x00000001,
		FLAG_2 = 0x00000002,
		FLAG_3 = 0x00000004,
		FLAG_4 = 0x00000008,
		FLAG_5 = 0x00000010,
		FLAG_6 = 0x00000020,
		FLAG_7 = 0x00000040,
		FLAG_8 = 0x00000080,
		FLAG_9 = 0x00000100,
		FLAG_10 = 0x00000200,
		FLAG_11 = 0x00000400,
		FLAG_12 = 0x00000800,
		FLAG_13 = 0x00001000,
		FLAG_14 = 0x00002000,
		FLAG_15 = 0x00004000,
		FLAG_16 = 0x00008000,
	};

	void error(std::string err, std::string file, std::string func, unsigned int line, int code = 0);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData);

	bool checkValidationLayerSupport();

	std::vector<const char*> getRequiredExtensions();

	VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	VkResult destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

	class App;

	struct QueueFamilyIndex {
		enum {
			NOT_FOUND = -1
		};
		QueueFamilyIndex();
		QueueFamilyIndex(App* app, VkPhysicalDevice device, int type);

		int graphicsFamily = NOT_FOUND;
		int presentFamily = NOT_FOUND;

		bool complete();
	};
	struct SwapChainDetails {

		SwapChainDetails(App* app, VkPhysicalDevice device);

		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		bool valid();
	};

	std::string getDeviceDetails(VkPhysicalDevice device);
	
	std::vector<char> readFile(const std::string& filepath);

	struct init {
		static void glfw(App* app);
		static void createInstance(App* app);
		static void createSurface(App* app);
		static void createDebugCallback(App* app);
		static void createDevice(App* app);
		static void createSwapChain(App* app);
		static void createImageViews(App* app);
		static void createGraphicsPipeline(App* app, std::string vertexShaderPath, std::string fragmentShaderPath);
		static void createRenderPass(App* app);
		static VkShaderModule createShaderModule(App* app, const std::vector<char>& binary);
		static void createFramebuffers(App* app);
		static void createCommandPool(App* app);
		static void createCommandBuffers(App* app);
		static void createSemaphores(App* app);
		static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		static bool isDeviceSuitable(App* app, VkPhysicalDevice device);
		static unsigned int scorePhysicalDevice(App* app, VkPhysicalDevice device);
		static void pickPhysicalDevice(App* app);
		static VkSurfaceFormatKHR chooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availible);
		static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availible, uint16_t flags);
		static VkExtent2D chooseSwapExtent(App* app, const VkSurfaceCapabilitiesKHR& capabilities);
	};
}