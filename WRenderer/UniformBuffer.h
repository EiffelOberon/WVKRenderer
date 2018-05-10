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

    void Upload(
        Device	      &device,
        const void	  *&data);

    vk::DescriptorBufferInfo GetBufferInfo() const;

private:

	vk::MemoryRequirements GetBufferMemoryRequirement(
		Device		&device,
		vk::Buffer	&buffer);

	int UniformBuffer::GetMemoryTypeIndex(
		const Device    				 &device,
		const vk::MemoryPropertyFlags	 memoryPropertyFlagBits,
		const vk::MemoryRequirements	 &memRequirement);

	vk::Buffer			        mBuffer;
	vk::DeviceMemory	        mMemory; 
    vk::DescriptorBufferInfo    mBufferInfo;
	vk::MemoryRequirements      mRequirement;
    size_t                      mSize;

    Device                      *mDevice;
};