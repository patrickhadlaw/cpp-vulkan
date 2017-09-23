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

#define CPPVK_VERSION_MAJOR 1
#define CPPVK_VERSION_MINOR 0
#define CPPVK_VERSION_PATCH 0

#define VULKAN_API_MAJOR 1
#define VULKAN_API_MINOR 0
#define VULKAN_API_PATCH 61

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

#ifdef NDEBUG
namespace cppvk {
	const bool enableValidationLayers = false;
}
#else
namespace cppvk {
	const bool enableValidationLayers = true;
}
#endif

namespace cppvk {
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
	void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

	class App;

	struct init {
		static void glfw(App* app);
		static void vkinstance(App* app);
		static void vkdebugCallback(App* app);
		static void pickPhysicalDevice(App* app);
	};
}