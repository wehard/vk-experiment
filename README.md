# vk-experiment

- Create `VkInstance`
- Select gfx card `VkPhysicalDevice`
- Create `VkDevice` and `VkQueue`
- Create window, window surface and swap chain
- Wrap swap chain images into `VkImageView`
- Create render pass (specifies render targets and usage)
- Create framebuffers for render pass
- Set up graphics pipeline
- Allocate and record a command buffer with draw commands for every swap chain image
- Draw frames by acquiring images, submitting draw command buffer and returning images to 	the swap chain
