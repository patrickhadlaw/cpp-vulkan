#include "init.h"
#include "app.h"

VKAPI_ATTR VkBool32 VKAPI_CALL cppvk::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData)
{
	std::cerr << "Error: (Vulkan validation layer: " << msg << ") object type: " << objType << " object: " << obj << " location: " << location << " code: " << code << std::endl;

	return VK_FALSE;
}

bool cppvk::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	std::cout << "Available vulkan validation layers:" << std::endl;

	for (const auto& layer : availableLayers) {
		std::cout << "\t" << layer.layerName << std::endl;
	}

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> cppvk::getRequiredExtensions()
{
	std::vector<const char*> extensions;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	if (cppvk::enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}

VkResult cppvk::createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugReportCallbackEXT * pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void cppvk::destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

void cppvk::init::glfw(App * app)
{
	if (!glfwInit()) {
		throw std::runtime_error("Error: failed to initialize glfw");
	}
	if (!glfwVulkanSupported())
	{
		throw std::runtime_error("Error: glfw vulkan not supported");
		glfwTerminate();
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	app->window = glfwCreateWindow(app->WIDTH, app->HEIGHT, "cpp-vulkan", nullptr, nullptr);
}

void cppvk::init::vkinstance(App * app)
{
	VkApplicationInfo appinfo = {};
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.pApplicationName = "cpp-vulkan";
	appinfo.applicationVersion = VK_MAKE_VERSION(CPPVK_VERSION_MAJOR, CPPVK_VERSION_MINOR, CPPVK_VERSION_PATCH);
	appinfo.pEngineName = "";
	appinfo.engineVersion = VK_MAKE_VERSION(CPPVK_VERSION_MAJOR, CPPVK_VERSION_MINOR, CPPVK_VERSION_PATCH);
	appinfo.apiVersion = VK_MAKE_VERSION(VULKAN_API_MAJOR, VULKAN_API_MINOR, VULKAN_API_PATCH);

	auto neededExtensions = cppvk::getRequiredExtensions();

	VkInstanceCreateInfo createinfo = {};
	createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createinfo.pApplicationInfo = &appinfo;
	createinfo.enabledExtensionCount = static_cast<uint32_t>(neededExtensions.size());
	createinfo.ppEnabledExtensionNames = neededExtensions.data();
	createinfo.enabledLayerCount = 0;

	VkResult result = vkCreateInstance(&createinfo, nullptr, &app->instance);

	if (result != VK_SUCCESS) {
		throw std::runtime_error("Error[" + std::to_string(result) + "]: failed to create VkInstance");
	}

	if (cppvk::enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	if (cppvk::enableValidationLayers) {
		createinfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createinfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createinfo.enabledLayerCount = 0;
	}

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "Available vulkan extensions:" << std::endl;

	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}
}

void cppvk::init::vkdebugCallback(App * app)
{
	if (!enableValidationLayers) {
		return;
	}
	VkDebugReportCallbackCreateInfoEXT createinfo = {};
	createinfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createinfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createinfo.pfnCallback = debugCallback;

	if (createDebugReportCallbackEXT(app->instance, &createinfo, nullptr, &app->callback) != VK_SUCCESS) {
		throw std::runtime_error("Error: failed to initialize debug callback");
	}
}
