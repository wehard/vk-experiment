/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utilities.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 18:10:56 by wkorande          #+#    #+#             */
/*   Updated: 2020/08/29 15:06:44 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <fstream>
#include <glm/glm.hpp>

const int MAX_FRAME_DRAWS = 2;

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 col;
};

// Indices (locations) of Queue Families

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentationFamily = -1;

	bool isValid()
	{
		return (graphicsFamily >= 0 && presentationFamily >= 0);
	}
};

struct SwapChainDetails
{
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentationModes; // e.g. mailbox, immediate etc. 
};

struct SwapchainImage
{
	VkImage image;
	VkImageView imageView;
};

static std::vector<char> readFile(const std::string &filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file!");
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> fileBuffer(fileSize);
	file.seekg(0); // move read pos to the start of file
	file.read(fileBuffer.data(), fileSize);
	file.close();
	return (fileBuffer);
}
