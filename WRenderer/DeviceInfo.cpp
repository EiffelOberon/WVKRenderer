#include "DeviceInfo.h"
#include "Error.h"

DeviceInfo::DeviceInfo(
    vk::Instance& instance, 
    vk::SurfaceKHR& surface)
	: mGpuCount(1)
{
	WERROR(instance.enumeratePhysicalDevices(&mGpuCount, nullptr),
        "Failed to get GPU count.");
	WASSERT((mGpuCount > 0), "Must have at least 1 GPU.");
	
    // get a list of physical VK devices
    std::vector<vk::PhysicalDevice> devices;
    devices.resize(mGpuCount);
    mDevices.resize(mGpuCount);
	WERROR(instance.enumeratePhysicalDevices(&mGpuCount, &devices[0]),
        "Failed to enumerate physical devices.");
    for (size_t i = 0; i < devices.size(); ++i)
    {
        mDevices[i].mPhysicalDevice = devices[i];
    }
    devices.clear();

	// get a list of graphics queue
	uint32_t queueFamilyCount = 0;
	std::vector<vk::QueueFamilyProperties> properties;
	for (size_t i = 0; i < mDevices.size(); ++i)
	{
		int graphicsFlagsCount = 0;
		vk::PhysicalDevice& device = mDevices[i].mPhysicalDevice;

        // get a list of queue family for the device
		device.getQueueFamilyProperties(&queueFamilyCount, nullptr);
		WASSERT((queueFamilyCount > 0), "Must have at least 1 queue.");
		properties.resize(queueFamilyCount);
		device.getQueueFamilyProperties(&queueFamilyCount, &properties[0]);
		WASSERT((queueFamilyCount > 0), "Must have at least 1 queue.");

		for (uint32_t j = 0; j < queueFamilyCount; ++j)
		{
            // 
			vk::QueueFamilyProperties& p = properties[j];
			if (p.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				vk::Bool32 supportPresent = false;
				WERROR(device.getSurfaceSupportKHR(j, surface, &supportPresent),
                    "Failed to get surface present support.");

                // only storing result for graphics capable queue's info for the GPU
                // ignoring the rest (TRANSFER / COMPUTE)
                mDevices[i].mPresentQueueFamilyIndex.push_back(j);
                mDevices[i].mGraphicsQueueFamilyIndex.push_back(j);
				mDevices[i].mSupportPresent |= supportPresent;
			}
		}

        // get a list of surface formats
        uint32_t surfaceFormatCount = 0;
        WERROR(device.getSurfaceFormatsKHR(surface, &surfaceFormatCount, nullptr),
            "Failed to get surface formats.");
        WASSERT((surfaceFormatCount > 0),
            "Surface format count must be greater than 0.");
        mDevices[i].mSurfaceFormats.resize(surfaceFormatCount);
        WERROR(device.getSurfaceFormatsKHR(surface, &surfaceFormatCount, &mDevices[i].mSurfaceFormats[0]),
            "Failed to get surface formats.");

		properties.clear();
	}

	// one queue for now, TODO expand this
	float queuePriorities[1] = { 0.0 };
	mDeviceQueueCreateInfo = vk::DeviceQueueCreateInfo()
		.setQueueCount(1)
		.setPQueuePriorities(queuePriorities)
		.setQueueFamilyIndex(mDevices[DEFAULT_GPU_INDEX].mGraphicsQueueFamilyIndex[DEFAULT_FAMILY_INDEX]);

	mDeviceCreateInfo = vk::DeviceCreateInfo()
		.setQueueCreateInfoCount(1)
		.setPQueueCreateInfos(&mDeviceQueueCreateInfo);

    mDeviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

DeviceInfo::~DeviceInfo()
{

}

Device& DeviceInfo::GetDevice(
    uint32_t index)
{
    return mDevices.at(index);
}

Device& DeviceInfo::CreateDevice(
    uint32_t gpuIndex)
{
    WERROR(mDevices[gpuIndex].mPhysicalDevice.createDevice(
        &mDeviceCreateInfo,
        nullptr,
        &mDevices[gpuIndex].mDevice),
        "Failed to create logical device.");
	return mDevices[gpuIndex];
}
