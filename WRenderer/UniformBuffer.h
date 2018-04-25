#pragma once

// libraries
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

class Device;
class UniformBuffer
{
public:
	UniformBuffer(
		Device			&device,
		const size_t	size);

	~UniformBuffer();

private:
	void Upload(
		Device	&device,
		void	*&data);

	vk::MemoryRequirements GetBufferMemoryRequirement(
		Device		&device,
		vk::Buffer	&buffer);

	int UniformBuffer::GetMemoryTypeIndex(
		const Device    				 &device,
		const vk::MemoryPropertyFlags	 memoryPropertyFlagBits,
		const vk::MemoryRequirements	 &memRequirement);

	vk::Buffer			   mBuffer;
	vk::DeviceMemory	   mMemory; 
	vk::MemoryRequirements mRequirement;
};