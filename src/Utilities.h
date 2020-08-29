/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utilities.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 18:10:56 by wkorande          #+#    #+#             */
/*   Updated: 2020/08/29 17:34:44 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <fstream>
#include <glm/glm.hpp>
#include <cstring>

const int MAX_FRAME_DRAWS = 2;

const std::vector<const char *> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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

static std::vector<Vertex> loadObjFile(std::string path)
{
	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec3> temp_colors;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<Vertex> vertices;

	FILE *f = fopen(path.c_str(), "r");
	if (!f)
		throw std::runtime_error("error opening obj file!");
	char buf[128];
	while (true)
	{
		int res = fscanf(f, "%s", buf);
		if (res == EOF)
			break;

		if (strcmp(buf, "v") == 0)
		{
			glm::vec3 pos;
			fscanf(f, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
			temp_positions.push_back(pos);
			temp_colors.push_back(glm::vec3{1.0, 1.0, 1.0});
		}
		else if (strcmp(buf, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(f, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(buf, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(f, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(buf, "f") == 0)
		{
			int v_i[3];
			int t_i[3];
			int n_i[3];
			fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v_i[0], &t_i[0], &n_i[0], &v_i[1], &t_i[1], &n_i[1], &v_i[2], &t_i[2], &n_i[2]);
			for (int i = 0; i < 3; i++)
			{
				v_i[i] -= 1;
				t_i[i] -= 1;
				n_i[i] -= 1;
				Vertex vertex = {};
				vertex.pos = glm::vec3{temp_positions[v_i[i]]};
				vertex.col = glm::vec3{temp_normals[n_i[i]]};
				vertices.push_back(vertex);
			}
			// m->indices.push_back(v_i[0]);
			// m->indices.push_back(v_i[1]);
			// m->indices.push_back(v_i[2]);
			
			// m->normals[(v_i[0] * 3)] = temp_normals[n_i[0]].x;
			// m->normals[(v_i[0] * 3) + 1] = temp_normals[n_i[0]].y;
			// m->normals[(v_i[0] * 3) + 2] = temp_normals[n_i[0]].z;

			// m->normals[(v_i[1] * 3)] = temp_normals[n_i[1]].x;
			// m->normals[(v_i[1] * 3) + 1] = temp_normals[n_i[1]].y;
			// m->normals[(v_i[1] * 3) + 2] = temp_normals[n_i[1]].z;

			// m->normals[(v_i[2] * 3)] = temp_normals[n_i[2]].x;
			// m->normals[(v_i[2] * 3) + 1] = temp_normals[n_i[2]].y;
			// m->normals[(v_i[2] * 3) + 2] = temp_normals[n_i[2]].z;
		}
	}
	return (vertices);
}

