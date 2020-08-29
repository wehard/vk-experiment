/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/29 13:21:58 by wkorande          #+#    #+#             */
/*   Updated: 2020/08/29 14:59:18 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mesh.h"
#include <cstring>

Mesh::Mesh()
{
}

Mesh::Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, std::vector<Vertex> *vertices)
{
	vertexCount = vertices->size();
	physicalDevice = newPhysicalDevice;
	device = newDevice;
	createVertexBuffer(vertices);
}

int Mesh::getVertexCount()
{
	return (vertexCount);
}

VkBuffer Mesh::getVertexBuffer()
{
	return (vertexBuffer);
}

void Mesh::destroyVertexBuffer()
{
	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);
}

Mesh::~Mesh()
{
}

void Mesh::createVertexBuffer(std::vector<Vertex> *vertices)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(Vertex) * vertices->size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer);
	if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to create vertex buffer!");

	// buffer memory requirements
	VkMemoryRequirements memoryRequirements = {};
	vkGetBufferMemoryRequirements(device, vertexBuffer, &memoryRequirements);

	// allocate memory to buffer
	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = findMemoryTypeIndex(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// allocate memory to device memory
	result = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &vertexBufferMemory);
	if (result != VK_SUCCESS)
		std::runtime_error("Failed to allocate vertex buffer memory!");

	// allocate meory to given vertex buffer
	vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);

	// map memory to vertex vuffer
	void *data;
	vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, vertices->data(), (size_t)bufferInfo.size);
	vkUnmapMemory(device, vertexBufferMemory);
}

uint32_t Mesh::findMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties)
{
	// get properties of physical device memory
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	// find index for required memory type
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((allowedTypes & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}
	throw std::runtime_error("Failed to find required memory type!");
	return (-1);
}
