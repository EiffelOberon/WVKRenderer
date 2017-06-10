#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

#include "DevConstants.h"
#include "DevStructs.h"

class DeviceInfo
{
private:
    Device& GetDevice(uint32_t index);

	uint32_t mGpuCount;										// number of GPUs on this computer
	std::vector<Device> mDevices;				            // array of VK physical devices

	vk::DeviceQueueCreateInfo mDeviceQueueCreateInfo;		// queue create info object
	vk::DeviceCreateInfo mDeviceCreateInfo;					// device create info object

    std::vector<std::string> mDeviceExtensionNames;

public:
	DeviceInfo(vk::Instance& instance, vk::SurfaceKHR& surface);
	~DeviceInfo();

	Device& CreateDevice(uint32_t gpuIndex);		            // creates a VK device
};