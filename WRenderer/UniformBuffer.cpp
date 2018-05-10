// myself
#include "UniformBuffer.h"

// includes
#include "DevConstants.h"
#include "Device.h"
#include "Error.h"

UniformBuffer::UniformBuffer(
    Device			&device,
    const size_t	size)
	: mBuffer(nullptr)
	, mMemory(nullptr)
	, mRequirement()
    , mSize(size)
    , mDevice(&device)
{
	vk::BufferCreateInfo bufferInfo;
	bufferInfo.setPNext(nullptr)
		.setUsage(vk::BufferUsageFlagBits::eUniformBuffer)
		.setSize(size)
		.setQueueFamilyIndexCount(0)
		.setPQueueFamilyIndices(nullptr)
		.setSharingMode(vk::SharingMode::eExclusive)
		.setFlags(vk::BufferCreateFlags());
	WERROR(device.GetDevice().createBuffer(&bufferInfo, nullptr, &mBuffer),
		"Failed to create uniform buffer.");

	// get memory requirement
	mRequirement = GetBufferMemoryRequirement(device, mBuffer);

	int memoryTypeIndex = GetMemoryTypeIndex(
		device, 
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, 
		mRequirement);

	WASSERT(memoryTypeIndex > 0, "Incorrect Memory Type Index.");

	vk::MemoryAllocateInfo bufferAllocInfo;
	bufferAllocInfo.setPNext(nullptr)
		.setMemoryTypeIndex(memoryTypeIndex)
		.setAllocationSize(mRequirement.size);

	WERROR(device.GetDevice().allocateMemory(&bufferAllocInfo, nullptr, &mMemory),
		"Failed to allocate device memory for depth buffer.");

	device.GetDevice().bindBufferMemory(mBuffer, mMemory, 0);

    mBufferInfo.buffer = mBuffer;
    mBufferInfo.offset = 0;
    mBufferInfo.range = mSize;
}

UniformBuffer::~UniformBuffer()
{
    mDevice->GetDevice().destroyBuffer(mBuffer, nullptr);
}

void UniformBuffer::Upload(
	Device					&device,
	const void		        *&data)
{
    void* writePtr;
    WERROR(device.GetDevice().mapMemory(mMemory, 0, mRequirement.size, vk::MemoryMapFlags(), (void**)&writePtr),
        "Failed to map GPU memory.");
    memcpy(writePtr, data, mSize);
	device.GetDevice().unmapMemory(mMemory);
}

vk::DescriptorBufferInfo UniformBuffer::GetBufferInfo() const
{
    return mBufferInfo;
}

vk::MemoryRequirements UniformBuffer::GetBufferMemoryRequirement(
	Device		&device, 
	vk::Buffer	&buffer)
{
	return device.GetDevice().getBufferMemoryRequirements(buffer);
}

int UniformBuffer::GetMemoryTypeIndex(
	const Device    				 &device,
	const vk::MemoryPropertyFlags	 memoryPropertyFlagBits,
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
