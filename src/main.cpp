/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 20:43:47 by wkorande          #+#    #+#             */
/*   Updated: 2020/08/27 21:45:32 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <cstring>

#include <glm/glm.hpp>

#include "VulkanRenderer.h"



GLFWwindow *window;
VulkanRenderer vulkanRenderer;

void initWindow(std::string title, int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // No api when using Vulkan
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

int main()
{
	initWindow("Vulkan", 1280, 720);
	if (vulkanRenderer.init(window) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwPollEvents();
	}
	vulkanRenderer.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();
}
