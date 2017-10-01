#include "init.h"
#include "app.h"

void cppvk::error(std::string err, std::string file, std::string func, unsigned int line)
{
	DEBUG_FUNCTION_MESSAGE
	std::string e = "\tError[file: " + file + "][func: " + func + "][line: " + std::to_string(line) + "]: " + err;
	throw std::runtime_error(e);
}

VKAPI_ATTR VkBool32 VKAPI_CALL cppvk::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData)
{
	DEBUG_FUNCTION_MESSAGE
	std::cerr << "\tError: (Vulkan validation layer: " << msg << ") object type: " << objType << " object: " << obj << " location: " << location << " code: " << code << std::endl;

	return VK_FALSE;
}

bool cppvk::checkValidationLayerSupport()
{
	DEBUG_FUNCTION_MESSAGE
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
	DEBUG_FUNCTION_MESSAGE
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
	DEBUG_FUNCTION_MESSAGE
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

VkResult cppvk::destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator)
{
	DEBUG_FUNCTION_MESSAGE
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
		return VK_SUCCESS;
	}
	return VK_INCOMPLETE;
}

void cppvk::init::glfw(App * app)
{
	DEBUG_FUNCTION_MESSAGE
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

void cppvk::init::createInstance(App * app)
{
	DEBUG_FUNCTION_MESSAGE
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

void cppvk::init::createSurface(App * app)
{
	DEBUG_FUNCTION_MESSAGE
	if (glfwCreateWindowSurface(app->instance, app->window, nullptr, &app->surface) != VK_SUCCESS) {
		cppvk::error("failed to create window surface", __FILE__, FUNC, __LINE__);
	}
}

void cppvk::init::createDebugCallback(App * app)
{
	DEBUG_FUNCTION_MESSAGE
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

void cppvk::init::createDevice(App * app)
{
	DEBUG_FUNCTION_MESSAGE
	QueueFamilyIndex index = QueueFamilyIndex(app, app->physicalDevice, VK_QUEUE_GRAPHICS_BIT);
	std::vector<VkDeviceQueueCreateInfo> queueCreateinfos = {};
	std::set<int> uniqueQueueFamilies = { index.graphicsFamily, index.presentFamily };

	float queuePriority = 1.0f;
	for (int i = 0; i < uniqueQueueFamilies.size(); i++) {
		VkDeviceQueueCreateInfo queueCreateinfo = {};
		queueCreateinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateinfo.queueFamilyIndex = index.graphicsFamily;
		queueCreateinfo.queueCount = 1;
		queueCreateinfo.pQueuePriorities = &queuePriority;
		queueCreateinfos.push_back(queueCreateinfo);
	}

	// Requested physical device features
	VkPhysicalDeviceFeatures enabledFeatures = {};
	
	VkDeviceCreateInfo deviceCreateinfo = {};
	deviceCreateinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateinfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateinfos.size());
	deviceCreateinfo.pQueueCreateInfos = queueCreateinfos.data();
	deviceCreateinfo.queueCreateInfoCount = 1;
	deviceCreateinfo.pEnabledFeatures = &enabledFeatures;
	deviceCreateinfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceCreateinfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (cppvk::enableValidationLayers) {
		deviceCreateinfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		deviceCreateinfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		deviceCreateinfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(app->physicalDevice, &deviceCreateinfo, nullptr, &app->device) != VK_SUCCESS) {
		cppvk::error("failed to create logical device", __FILE__, FUNC, __LINE__);
	}

	vkGetDeviceQueue(app->device, index.graphicsFamily, 0, &app->graphicsQueue);
	vkGetDeviceQueue(app->device, index.presentFamily, 0, &app->presentQueue);
}

void cppvk::init::createSwapChain(App * app)
{
	DEBUG_FUNCTION_MESSAGE
	SwapChainDetails swapChainDetails = SwapChainDetails(app, app->physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapChainSurfaceFormat(swapChainDetails.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainDetails.presentModes, app->flags);
	VkExtent2D extent = chooseSwapExtent(app, swapChainDetails.capabilities);

	// Add one to image count to allow for triple buffering functionality, checking if the amount of swap buffer images we want to use are availible
	uint32_t imageCount = swapChainDetails.capabilities.minImageCount + 1;
	if (swapChainDetails.capabilities.maxImageCount > 0 && imageCount > swapChainDetails.capabilities.maxImageCount) { // If max image count is zero there is no limit of images
		imageCount = swapChainDetails.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createinfo = {};
	createinfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createinfo.surface = app->surface;
	createinfo.minImageCount = imageCount;
	createinfo.imageFormat = surfaceFormat.format;
	createinfo.imageColorSpace = surfaceFormat.colorSpace;
	createinfo.imageExtent = extent;
	createinfo.imageArrayLayers = 1; // There is only one layer of image
	createinfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndex index = QueueFamilyIndex(app, app->physicalDevice, VK_QUEUE_GRAPHICS_BIT);
	uint32_t queueFamilyIndicies[] = { (uint32_t)index.graphicsFamily, (uint32_t)index.presentFamily };

	if (index.graphicsFamily != index.presentFamily) {
		createinfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createinfo.queueFamilyIndexCount = 2;
		createinfo.pQueueFamilyIndices = queueFamilyIndicies;
	}
	else {
		createinfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createinfo.queueFamilyIndexCount = 0;
		createinfo.pQueueFamilyIndices = nullptr;
	}
	createinfo.preTransform = swapChainDetails.capabilities.currentTransform;
	createinfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createinfo.presentMode = presentMode;
	createinfo.clipped = VK_TRUE;

	createinfo.oldSwapchain = VK_NULL_HANDLE; // When swap chain is modified (window resized) the swap chain will become unoptimal and will need to be remade
	
	if (vkCreateSwapchainKHR(app->device, &createinfo, nullptr, &app->swapChain) != VK_SUCCESS) {
		cppvk::error("failed to create swap chain", __FILE__, FUNC, __LINE__);
	}

	app->swapChainImageFormat = surfaceFormat.format;
	app->swapChainExtent = extent;

	vkGetSwapchainImagesKHR(app->device, app->swapChain, &imageCount, nullptr);
	app->swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(app->device, app->swapChain, &imageCount, app->swapChainImages.data());
}

void cppvk::init::createImageViews(App * app)
{

}

bool cppvk::init::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	DEBUG_FUNCTION_MESSAGE
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> properties(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, properties.data());

	std::vector<const char*> required = deviceExtensions;

	bool extensionsFound = false;
	for (int i = 0; i < properties.size(); i++) {
		for (int j = 0; j < required.size(); j++) {
			if (properties[i].extensionName == required[j]) {
				required.erase(required.begin() + j);
			}
		}
	}

	return required.empty();
}

std::string cppvk::getDeviceDetails(VkPhysicalDevice device)
{
	DEBUG_FUNCTION_MESSAGE
	std::string det = "";
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);
	det = "Physical device: ";
	det = det + properties.deviceName;
	det = det + "\n\tType: " + std::to_string(properties.deviceType);

	return det;
}

bool cppvk::init::isDeviceSuitable(App* app, VkPhysicalDevice device)
{
	DEBUG_FUNCTION_MESSAGE
	QueueFamilyIndex index = QueueFamilyIndex(app, device, VK_QUEUE_GRAPHICS_BIT);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainIsSuitable = false;
	if (extensionsSupported) {
		SwapChainDetails details = SwapChainDetails(app, device);
		swapChainIsSuitable = details.valid();
	}
	
	return index.complete() && extensionsSupported && swapChainIsSuitable;
}

unsigned int cppvk::init::scorePhysicalDevice(App* app, VkPhysicalDevice device)
{
	DEBUG_FUNCTION_MESSAGE
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);
	unsigned int score = 0;
	if (features.geometryShader) {
		score += 10;
	}
	if (features.alphaToOne) {
		score += 5;
	}
	if (features.tessellationShader) {
		score += 10;
	}

	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);

	score += properties.limits.maxImageDimension2D;
	
	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score = score + 1000;
	}
	else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
		score = score + 100;
	}
	else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) {
		score = score + 10;
	}

	if (!features.geometryShader) {
		score = 0;
	}

	return score;
}

void cppvk::init::pickPhysicalDevice(App * app)
{
	DEBUG_FUNCTION_MESSAGE
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(app->instance, &deviceCount, nullptr);

	if (deviceCount <= 0) {
		cppvk::error("physical device count is zero", __FILE__, FUNC, __LINE__);
	}
	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	vkEnumeratePhysicalDevices(app->instance, &deviceCount, physicalDevices.data());

	VkPhysicalDevice bestDevice = nullptr;
	unsigned int currentScore = 0;
	for (int i = 0; i < physicalDevices.size(); i++){
		unsigned int newScore = cppvk::init::scorePhysicalDevice(app, physicalDevices[i]);
		if (isDeviceSuitable(app, physicalDevices[i])) {

		}
		else if (newScore > currentScore) {
			bestDevice = physicalDevices[i];
			currentScore = newScore;
		}
	}

	if (currentScore == 0) {
		if (bestDevice != nullptr) {
			cppvk::error("no suitable devices detected: " + cppvk::getDeviceDetails(bestDevice), __FILE__, FUNC, __LINE__);
		}
		else {
			cppvk::error("no suitable devices detected", __FILE__, FUNC, __LINE__);
		}
	}
	else {
		app->physicalDevice = bestDevice;
	}
}

VkSurfaceFormatKHR cppvk::init::chooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availible)
{
	DEBUG_FUNCTION_MESSAGE
	// When only one format is availible and it is format undefined it does not prefer a specific format
	if (availible.size() == 1 && availible[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}
	for (int i = 0; i < availible.size(); i++) {
		// If surface format provides ideal settings then give format
		if (availible[i].format == VK_FORMAT_B8G8R8A8_UNORM && availible[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availible[i];
		}
	}

	return availible[0];
}

VkPresentModeKHR cppvk::init::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availible, uint16_t flags)
{
	DEBUG_FUNCTION_MESSAGE
	VkPresentModeKHR targetPresentMode;
	if (flags & cppvk::USE_IMMEDIATE_SWAP) {
		targetPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	}
	else if (flags & cppvk::USE_RELAXED_VSYNC_SWAP) {
		targetPresentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
	}
	else if (flags & cppvk::USE_VSYNC_SWAP) {
		targetPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	}
	else if (flags & cppvk::USE_TRIPLE_BUFFER_SWAP) {
		targetPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
	}

	for (int i = 0; i < availible.size(); i++) {
		if (availible[i] == targetPresentMode) {
			return targetPresentMode;
		}
	}
	return VK_PRESENT_MODE_IMMEDIATE_KHR;
}

VkExtent2D cppvk::init::chooseSwapExtent(App* app, const VkSurfaceCapabilitiesKHR & capabilities)
{
	DEBUG_FUNCTION_MESSAGE
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D extent = {app->WIDTH, app->HEIGHT};
		// Chooses the smaller extent width between extent max and window height then chooses max of min extent width and previous choice
		extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, extent.width));
		// Chooses the smaller extent height between extent max and window height then chooses max of min extent height and previous choice
		extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, extent.height));

		return extent;
	}
}

cppvk::QueueFamilyIndex::QueueFamilyIndex()
{
	DEBUG_FUNCTION_MESSAGE
	graphicsFamily = NOT_FOUND;
}

cppvk::QueueFamilyIndex::QueueFamilyIndex(App* app, VkPhysicalDevice device, int type)
{
	DEBUG_FUNCTION_MESSAGE
	graphicsFamily = NOT_FOUND;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

	for (int i = 0; i < queueFamilyProperties.size(); i++) {
		// If found queue family thats supports graphics
		if (queueFamilyProperties[i].queueCount > 0 && queueFamilyProperties[i].queueFlags & type) {
			graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i), app->surface, &presentSupport);
		if (queueFamilyProperties[i].queueCount > 0 && presentSupport) {
			presentFamily = i;
		}

		if (complete()) {
			break;
		}
	}
}

bool cppvk::QueueFamilyIndex::complete()
{
	DEBUG_FUNCTION_MESSAGE
	return graphicsFamily >= 0 && presentFamily >= 0;
}

cppvk::SwapChainDetails::SwapChainDetails(App * app, VkPhysicalDevice device)
{
	DEBUG_FUNCTION_MESSAGE
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, app->surface, &capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, app->surface, &formatCount, nullptr);

	if (formatCount != 0) {
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, app->surface, &formatCount, formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, app->surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, app->surface, &presentModeCount, presentModes.data());
	}
}

bool cppvk::SwapChainDetails::valid()
{
	DEBUG_FUNCTION_MESSAGE
	return !formats.empty() && !presentModes.empty();
}
