#include "DeviceInfo.h"
#include "Error.h"

DeviceInfo::DeviceInfo(vk::Instance& instance, vk::SurfaceKHR& surface)
	: mGpuCount(1)
{
	vk::Result res = instance.enumeratePhysicalDevices(&mGpuCount, nullptr);
	WASSERT(mGpuCount > 0 && res == vk::Result::eSuccess, "Must have at least 1 GPU.");
	mDevices.resize(mGpuCount);
	mDeviceGraphicsQueue.resize(mGpuCount);
	mDeviceSupportPresent.resize(mGpuCount);
	res = instance.enumeratePhysicalDevices(&mGpuCount, &mDevices[0]);
	WASSERT(res == vk::Result::eSuccess, "Failed to enumerate physical devices.");

	// get a list of graphics queue
	uint32_t queueFamilyCount = 0;
	std::vector<vk::QueueFamilyProperties> properties;
	for (size_t i = 0; i < mDevices.size(); ++i)
	{
		int graphicsFlagsCount = 0;
		vk::PhysicalDevice& device = mDevices[i];

		device.getQueueFamilyProperties(&queueFamilyCount, nullptr);
		WASSERT(queueFamilyCount > 0, "Must have at least 1 queue.");
		properties.resize(queueFamilyCount);
		device.getQueueFamilyProperties(&queueFamilyCount, &properties[0]);
		WASSERT(queueFamilyCount > 0, "Must have at least 1 queue.");

		for (size_t j = 0; j < properties.size(); ++j)
		{
			vk::QueueFamilyProperties& p = properties[j];
			if (p.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				vk::Bool32 supportPresent = false;
				vk::Result res = device.getSurfaceSupportKHR(j, surface, &supportPresent);
				WASSERT(res == vk::Result::eSuccess, "Failed to get surfae present support.")

				// only storing result for graphics capable queue's info for the GPU
				// ignoring the rest (TRANSFER / COMPUTE)
				mDeviceSupportPresent[i] = supportPresent;
				mDeviceGraphicsQueue[i] = j;
				break;
			}
		}
		properties.clear();
	}

	// one queue for now, TODO expand this
	float queuePriorities[1] = { 0.0 };
	mDeviceQueueCreateInfo = vk::DeviceQueueCreateInfo()
		.setQueueCount(1)
		.setPQueuePriorities(queuePriorities)
		.setQueueFamilyIndex(mDeviceGraphicsQueue[DEFAULT_GPU_INDEX]);

	mDeviceCreateInfo = vk::DeviceCreateInfo()
		.setQueueCreateInfoCount(1)
		.setPQueueCreateInfos(&mDeviceQueueCreateInfo);
}

DeviceInfo::~DeviceInfo()
{

}

std::unique_ptr<vk::Device> DeviceInfo::CreateDevice(uint32_t gpuIndex)
{
	// create with the first gpu
	std::unique_ptr<vk::Device> device = std::make_unique<vk::Device>();
	vk::Result res = mDevices[gpuIndex].createDevice(&mDeviceCreateInfo, nullptr, device.get());
	WASSERT(res == vk::Result::eSuccess, "Failed to create device.");

	return std::move(device);
}
