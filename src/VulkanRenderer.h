/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VulkanRenderer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:08:26 by wkorande          #+#    #+#             */
/*   Updated: 2020/08/27 23:38:22 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <set>
#include <algorithm>

#include "Utilities.h"

#define VK_DEBUG 1
#define NVIDIA_VID 4318

class VulkanRenderer
{
public:
	VulkanRenderer();
	int init(GLFWwindow *newWindow);
	void cleanup();
	void run();
	~VulkanRenderer();

private:
	GLFWwindow *window;

	// Vulkan
	VkInstance instance;
	struct
	{
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	} mainDevice;
	VkQueue graphicsQueue;
	VkQueue presentationQueue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapchain;

	void createInstance();
	void createLogicalDevice();
	void createSurface();
	void createSwapchain();

	void getPhysicalDevice();

	bool checkInstanceExtensionSupport(std::vector<const char *> *checkExtensions);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkDeviceSuitable(VkPhysicalDevice device);

	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);

	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities);

#ifdef VK_DEBUG
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif

	const std::vector<const char *> validationLayers = {
		"VK_LAYER_KHRONOS_validation"};
	bool checkValidationLayerSupport();
};
