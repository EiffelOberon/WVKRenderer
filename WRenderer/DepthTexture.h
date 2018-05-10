#pragma once

// libraries
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

class Device;
class DepthTexture
{
public:
	DepthTexture(
		Device				&device,
		const uint32_t		width,
		const uint32_t		height);

	~DepthTexture();

private:
	vk::MemoryRequirements GetImageMemoryRequirement(
		Device					&device,
		vk::Image				&image);

	int GetMemoryTypeIndex(
		const Device    				 &device,
		const vk::MemoryPropertyFlagBits memoryPropertyFlagBits,
		const vk::MemoryRequirements	 &memRequirement);

    // vulkan objects
	vk::Image			mImage;
	vk::ImageView		mImageView;
	vk::DeviceMemory	mDeviceMem;

    // weak pointer
    Device              *mDevice;
};