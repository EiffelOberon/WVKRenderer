#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>

#define DEFAULT_GPU_INDEX 0

class DeviceInfo
{
private:
	uint32_t mGpuCount;										// number of GPUs on this computer
	std::vector<vk::PhysicalDevice> mDevices;				// array of VK physical devices
	std::vector<uint32_t> mDeviceGraphicsQueue;				// array of queue family indices for graphics
	std::vector<bool> mDeviceSupportPresent;				// array of boolean of devices supporting present

	vk::DeviceQueueCreateInfo mDeviceQueueCreateInfo;		// queue create info object
	vk::DeviceCreateInfo mDeviceCreateInfo;					// device create info object

public:
	DeviceInfo(vk::Instance& instance, vk::SurfaceKHR& surface);
	~DeviceInfo();

	std::unique_ptr<vk::Device> CreateDevice(uint32_t gpuIndex);		// creates a VK device
};