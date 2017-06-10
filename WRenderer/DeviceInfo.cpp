#include "DeviceInfo.h"
#include "Error.h"

DeviceInfo::DeviceInfo(vk::Instance& instance, vk::SurfaceKHR& surface)
	: mGpuCount(1)
{
	vk::Result res = instance.enumeratePhysicalDevices(&mGpuCount, nullptr);
	WASSERT(mGpuCount > 0 && res == vk::Result::eSuccess, "Must have at least 1 GPU.");
	
    // get a list of physical VK devices
    std::vector<vk::PhysicalDevice> devices;
    devices.resize(mGpuCount);
	res = instance.enumeratePhysicalDevices(&mGpuCount, &devices[0]);
    InitPhysicalDevices(devices);
    devices.clear();

	WASSERT(res == vk::Result::eSuccess, "Failed to enumerate physical devices.");

	// get a list of graphics queue
	uint32_t queueFamilyCount = 0;
	std::vector<vk::QueueFamilyProperties> properties;
	for (size_t i = 0; i < mDevices.size(); ++i)
	{
		int graphicsFlagsCount = 0;
		vk::PhysicalDevice& device = mDevices[i].mDevice;

        // get a list of queue family for the device
		device.getQueueFamilyProperties(&queueFamilyCount, nullptr);
		WASSERT(queueFamilyCount > 0, "Must have at least 1 queue.");
		properties.resize(queueFamilyCount);
		device.getQueueFamilyProperties(&queueFamilyCount, &properties[0]);
		WASSERT(queueFamilyCount > 0, "Must have at least 1 queue.");

		for (uint32_t j = 0; j < queueFamilyCount; ++j)
		{
            // 
			vk::QueueFamilyProperties& p = properties[j];
			if (p.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				vk::Bool32 supportPresent = false;
				res = device.getSurfaceSupportKHR(j, surface, &supportPresent);
                WASSERT(res == vk::Result::eSuccess, "Failed to get surface present support.")

                // only storing result for graphics capable queue's info for the GPU
                // ignoring the rest (TRANSFER / COMPUTE)
                mDevices[i].mPresentQueueFamilyIndex.push_back(j);
                mDevices[i].mGraphicsQueueFamilyIndex.push_back(j);
				mDevices[i].mSupportPresent |= supportPresent;
			}
		}

        // get a list of surface formats
        uint32_t surfaceFormatCount = 0;
        res = device.getSurfaceFormatsKHR(surface, &surfaceFormatCount, nullptr);
        WASSERT(res == vk::Result::eSuccess && surfaceFormatCount > 0, "Failed to get surface formats.");
        mDevices[i].mSurfaceFormats.resize(surfaceFormatCount);
        res = device.getSurfaceFormatsKHR(surface, &surfaceFormatCount, &mDevices[i].mSurfaceFormats[0]);
        
        std::vector<vk::SurfaceCapabilitiesKHR> surfaceCapabilities;
        res = device.getSurfaceCapabilitiesKHR(surface, &surfaceCapabilities[0]);
        WASSERT(res == vk::Result::eSuccess, "Failed to get surface capabilities");



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


void DeviceInfo::InitPhysicalDevices(std::vector<vk::PhysicalDevice>& devices)
{
    mDevices.resize(devices.size());
    for (size_t i = 0; i < devices.size(); ++i)
    {
        mDevices[i].mDevice = devices[i];
    }
}


std::unique_ptr<vk::Device> DeviceInfo::CreateDevice(uint32_t gpuIndex)
{
	// create with the first gpu
	std::unique_ptr<vk::Device> device = std::make_unique<vk::Device>();
	vk::Result res = mDevices[gpuIndex].mDevice.createDevice(&mDeviceCreateInfo, nullptr, device.get());
	WASSERT(res == vk::Result::eSuccess, "Failed to create device.");

	return std::move(device);
}
