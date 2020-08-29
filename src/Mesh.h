/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/29 13:22:06 by wkorande          #+#    #+#             */
/*   Updated: 2020/08/29 14:59:34 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include "Utilities.h"

class Mesh
{
public:
	Mesh();
	
	Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, std::vector<Vertex> *vertices);
	int getVertexCount();
	VkBuffer getVertexBuffer();
	void destroyVertexBuffer();
	~Mesh();

	private:
	int vertexCount;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	
	VkPhysicalDevice physicalDevice;
	VkDevice device;

	void createVertexBuffer(std::vector<Vertex> *vertices);
	uint32_t findMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties);
};
