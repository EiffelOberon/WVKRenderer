// myself
#include "DepthTexture.h"
// includes
#include "DevConstants.h"
#include "Device.h"
#include "Error.h"

DepthTexture::DepthTexture(
	Device				&device,
	const uint32_t		width,
	const uint32_t		height)
{
	// specify format
	vk::Format format = vk::Format::eD16Unorm;

	vk::ImageCreateInfo depthInfo;
	depthInfo.setPNext(nullptr)
		.setImageType(vk::ImageType::e2D)
		.setFormat(format)
		.setExtent(vk::Extent3D(width, height, 1))
		.setMipLevels(1)
		.setArrayLayers(1)
		.setSamples(vk::SampleCountFlagBits::e1)
		.setInitialLayout(vk::ImageLayout::eUndefined)
		.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment)
		.setQueueFamilyIndexCount(0)
		.setPQueueFamilyIndices(nullptr)
		.setSharingMode(vk::SharingMode::eExclusive)
		.setFlags(vk::ImageCreateFlags());

	// create vulkan object but not allocate yet
	WERROR(device.GetDevice().createImage(&depthInfo, nullptr, &mImage),
		"Failed to create depth buffer");

	// get the memory requirement
	vk::MemoryRequirements memReq = GetImageMemoryRequirement(device, mImage);

	// get memory type index, use the memory properties to determine the type of memory required
	int memTypeIndex = GetMemoryTypeIndex(device, vk::MemoryPropertyFlagBits::eDeviceLocal, memReq);
	WASSERT(memTypeIndex > 0, "Incorrect Memory Type Index.");

	// do allocation
	vk::MemoryAllocateInfo memAllocInfo;
	memAllocInfo.setPNext(nullptr)
		.setAllocationSize(memReq.size)
		.setMemoryTypeIndex(memTypeIndex);

	WERROR(device.GetDevice().allocateMemory(&memAllocInfo, nullptr, &mDeviceMem),
		"Failed to allocate device memory for depth buffer.");

	// bind allocated memory to the object
	device.GetDevice().bindImageMemory(mImage, mDeviceMem, 0);

	vk::ImageViewCreateInfo viewInfo;
	viewInfo.setPNext(nullptr)
		.setImage(mImage)
		.setFormat(format)
		.setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA))
		.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1))
		.setFlags(vk::ImageViewCreateFlags());

	WERROR(device.GetDevice().createImageView(&viewInfo, nullptr, &mImageView),
		"Failed to create depth image view.");
}

DepthTexture::~DepthTexture()
{

}


vk::MemoryRequirements DepthTexture::GetImageMemoryRequirement(
	Device					&device,
	vk::Image				&image)
{
	return device.GetDevice().getImageMemoryRequirements(image);
}


int DepthTexture::GetMemoryTypeIndex(
	const Device    				 &device,
	const vk::MemoryPropertyFlagBits memoryPropertyFlagBits,
	const vk::MemoryRequirements	 &memRequirement)
{
	uint32_t typeBits = memRequirement.memoryTypeBits;
	vk::PhysicalDeviceMemoryProperties properties = device.GetPhysicalDevice().getMemoryProperties();
	for (uint32_t i = 0; i < properties.memoryTypeCount; ++i)
	{
		if ((typeBits & 1) == 1)
		{
			vk::MemoryPropertyFlags bits = properties.memoryTypes[i].propertyFlags & memoryPropertyFlagBits;
			if (bits == memoryPropertyFlagBits)
			{
				return i;
			}
		}
		typeBits >>= 1;
	}

	return -1;
}
