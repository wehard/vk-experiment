/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VulkanRenderer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:07:42 by wkorande          #+#    #+#             */
/*   Updated: 2020/08/27 23:40:24 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VulkanRenderer.h"
#include <iostream>
#include <string.h>

VulkanRenderer::VulkanRenderer()
{
}

int VulkanRenderer::init(GLFWwindow *newWindow)
{
	window = newWindow;
	try
	{
		createInstance();
		// debug callback should be created here
		createSurface();
		getPhysicalDevice();
		createLogicalDevice();
		createSwapchain();
	}
	catch (const std::runtime_error &e)
	{
		printf("Error: %s\n", e.what());
		return (EXIT_FAILURE);
	}
	return (0);
}

void VulkanRenderer::cleanup()
{
	vkDestroySwapchainKHR(mainDevice.logicalDevice, swapchain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyDevice(mainDevice.logicalDevice, nullptr);
	vkDestroyInstance(instance, nullptr);
}

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::createInstance()
{
	if (this->enableValidationLayers && !checkValidationLayerSupport())
		throw std::runtime_error("Validation layers requested, but not available!");

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Application";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char *> instanceExtensions = std::vector<const char *>();
	uint32_t glfwExtensionCount = 0;
	const char **glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}
	if (!checkInstanceExtensionSupport(&instanceExtensions))
		throw std::runtime_error("VkInstance does not support required extensions!");
	createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// Validation layers setup

	if (this->enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
		createInfo.ppEnabledLayerNames = this->validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create VkInstance!");
	}
}

void VulkanRenderer::createLogicalDevice()
{
	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> queueFamilyIndices = {indices.graphicsFamily, indices.presentationFamily};

	for (int queueFamilyIndex : queueFamilyIndices)
	{

		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = 1;
		float priority = 1.0f;
		queueCreateInfo.pQueuePriorities = &priority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

	VkPhysicalDeviceFeatures deviceFeatures = {}; // empty for now
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Logical Device!");
	}

	// Get handle to graphics queue
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.presentationFamily, 0, &presentationQueue);
}

void VulkanRenderer::createSurface()
{
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
	if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to create Vulkan surface!");
}

void VulkanRenderer::createSwapchain()
{
	SwapChainDetails swapChainDetails = getSwapChainDetails(mainDevice.physicalDevice);

	// optimal surface format, presentation mode, extents
	VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormat(swapChainDetails.formats);
	VkPresentModeKHR presentMode = chooseBestPresentationMode(swapChainDetails.presentationModes);
	VkExtent2D extent = chooseSwapExtent(swapChainDetails.surfaceCapabilities);

	// one more for triple buffering
	uint32_t imageCount = swapChainDetails.surfaceCapabilities.minImageCount + 1;

	if (swapChainDetails.surfaceCapabilities.maxImageCount > 0 && swapChainDetails.surfaceCapabilities.maxImageCount < imageCount)
		imageCount = swapChainDetails.surfaceCapabilities.maxImageCount;

	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = surface;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.presentMode = presentMode;
	swapChainCreateInfo.imageExtent = extent;
	swapChainCreateInfo.minImageCount = imageCount;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainCreateInfo.preTransform = swapChainDetails.surfaceCapabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.clipped = VK_TRUE;

	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);
	// needs to be shared if they are different
	if (indices.graphicsFamily != indices.presentationFamily)
	{
		uint32_t queueFamilyIndices[]{
			(uint32_t)indices.graphicsFamily,
			(uint32_t)indices.presentationFamily};
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
	}
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	VkResult result = vkCreateSwapchainKHR(mainDevice.logicalDevice, &swapChainCreateInfo, nullptr, &swapchain);
	if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to create swapchain!");
}

void VulkanRenderer::getPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("No GPUs that support Vulkan!");
	}

	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

	for (const auto &device : deviceList)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		if (checkDeviceSuitable(device) && deviceProperties.vendorID == NVIDIA_VID)
		{
			printf("Using device: %s\n", deviceProperties.deviceName);
			mainDevice.physicalDevice = device;
			break;
		}
	}
}

bool VulkanRenderer::checkInstanceExtensionSupport(std::vector<const char *> *checkExtensions)
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	for (const auto &checkExtension : *checkExtensions)
	{
		bool hasExtension = false;
		for (const auto &extension : extensions)
		{
			if (strcmp(checkExtension, extension.extensionName))
			{
				hasExtension = true;
				break;
			}
		}
		if (!hasExtension)
			return (false);
	}
	return (true);
}

bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	if (extensionCount == 0)
		return (false);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

	for (const auto deviceExtension : deviceExtensions)
	{
		bool hasExtension = false;
		for (const auto &extension : extensions)
		{
			if (strcmp(deviceExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}
		if (!hasExtension)
			return (false);
	}
	return (true);
}

bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
{
	/*
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	
	VkPhysicalDeviceFeatures dewviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &dewviceFeatures);
	*/
	QueueFamilyIndices indices = getQueueFamilies(device);
	bool extensionSupported = checkDeviceExtensionSupport(device);
	bool swapChainValid = false;
	if (extensionSupported)
	{
		SwapChainDetails SwapChainDetails = getSwapChainDetails(device);
		swapChainValid = !SwapChainDetails.presentationModes.empty() && !SwapChainDetails.formats.empty();
	}
	return (indices.isValid() && extensionSupported && swapChainValid);
}

QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;
	uint32_t queueFamilycount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilycount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilycount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilycount, queueFamilyList.data());
	int i = 0;
	for (const auto &queueFamily : queueFamilyList)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}
		// check if queue family supports presentation
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationSupport);
		if (queueFamily.queueCount > 0 && presentationSupport)
			indices.presentationFamily = i;
		if (indices.isValid())
			break;
		i++;
	}
	return (indices);
}

SwapChainDetails VulkanRenderer::getSwapChainDetails(VkPhysicalDevice device)
{
	SwapChainDetails swapChainDetails;

	// Get surface capabilities for physical device
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainDetails.surfaceCapabilities);

	// Formats
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainDetails.formats.data());
	}
	// Presentation modes
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, nullptr);
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, swapChainDetails.presentationModes.data());
	}
	return (swapChainDetails);
}

// Format could be any but for now:
// format		:	VK_FORMAT_R8G8B8A8_UNORM
// colorSpace	:	VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
VkSurfaceFormatKHR VulkanRenderer::chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats)
{
	VkSurfaceFormatKHR surfaceFormat = {};
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
	{
		surfaceFormat.format = VK_FORMAT_R8G8B8A8_UNORM;
		surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		return (surfaceFormat);
	}
	for (const auto &format : formats)
	{
		if ((format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return (format);
	}
	return (formats[0]);
}

VkPresentModeKHR VulkanRenderer::chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes)
{
	for (const auto &presentationMode : presentationModes)
	{
		if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return (presentationMode);
	}
	return (VK_PRESENT_MODE_FIFO_KHR);
}

VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities)
{
	VkExtent2D extent = {};

	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return (surfaceCapabilities.currentExtent);
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		extent.width = static_cast<uint32_t>(width);
		extent.height = static_cast<uint32_t>(height);
		// check that extents are within bound set by surface capabilities
		extent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, extent.width));
		extent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, extent.height));
		return (extent);
	}
	return (extent);
}

bool VulkanRenderer::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char *layerName : this->validationLayers)
	{
		bool layerFound = false;

		for (const auto &layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}
