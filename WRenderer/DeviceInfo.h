#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

#include "DevConstants.h"
#include "Device.h"
#include "DevStructs.h"

class DeviceInfo
{
public:
	DeviceInfo(vk::Instance& instance, vk::SurfaceKHR& surface);
	~DeviceInfo();

	Device& GetDevice(uint32_t index);
	Device& CreateDevice(uint32_t gpuIndex);		        // creates a VK device

private:

	uint32_t            mGpuCount;							// number of GPUs on this computer
	float	            mQueuePriority;
	std::vector<Device> mDevices;				            // array of VK physical devices

	vk::DeviceQueueCreateInfo mDeviceQueueCreateInfo;		// queue create info object
	vk::DeviceCreateInfo      mDeviceCreateInfo;		    // device create info object

    std::vector<const char*>  mDeviceExtensionNames;
};