#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

#include "DevConstants.h"

struct Device
{
    vk::PhysicalDevice mDevice;
    vk::Bool32 mSupportPresent;
    std::vector<uint32_t> mGraphicsQueueFamilyIndex;    // Queue Family Index that supports Graphics
    std::vector<uint32_t> mPresentQueueFamilyIndex;     // Queue Family Index that supports Present
    std::vector<vk::SurfaceFormatKHR> mSurfaceFormats;   // Surface formats supported by the Device
};

class DeviceInfo
{
private:
    void InitPhysicalDevices(std::vector<vk::PhysicalDevice>& devices);

	uint32_t mGpuCount;										// number of GPUs on this computer
	std::vector<Device> mDevices;				            // array of VK physical devices

	vk::DeviceQueueCreateInfo mDeviceQueueCreateInfo;		// queue create info object
	vk::DeviceCreateInfo mDeviceCreateInfo;					// device create info object

    std::vector<std::string> mDeviceExtensionNames;

public:
	DeviceInfo(vk::Instance& instance, vk::SurfaceKHR& surface);
	~DeviceInfo();

	std::unique_ptr<vk::Device> CreateDevice(uint32_t gpuIndex);		// creates a VK device
};