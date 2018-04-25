#pragma once

// libraries
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

/// Device object
class Device
{
public:
	Device();
	~Device();

	vk::Bool32							mSupportPresent;              // whether Present is supported

	vk::Device& GetDevice() { return mDevice; }
	void SetDevice(vk::Device d) { mDevice = d; }

	const vk::PhysicalDevice& GetPhysicalDevice() const { return mPhysicalDevice; }
	void SetPhysicalDevice(vk::PhysicalDevice d) { mPhysicalDevice = d; }

	const uint32_t GetGraphicsQueueFamilyIndex(const uint32_t index) const { return mGraphicsQueueFamilyIndex[index]; }
	void AddGraphicsQueueFamilyIndex(const uint32_t index) { mGraphicsQueueFamilyIndex.push_back(index); }

	const uint32_t GetPresentQueueFamilyIndex(const uint32_t index) const { return mPresentQueueFamilyIndex[index]; }
	void AddPresentQueueFamilyIndex(const uint32_t index) { mPresentQueueFamilyIndex.push_back(index); }

	vk::SurfaceFormatKHR& GetSurfaceFormat(const uint32_t index) { return mSurfaceFormats[index]; }
	void SetSurfaceFormatCount(const uint32_t size) { mSurfaceFormats.resize(size); }

private:
	vk::Device							mDevice;                      // Logical device object
	vk::PhysicalDevice					mPhysicalDevice;              // Physical device object
	std::vector<uint32_t>				mGraphicsQueueFamilyIndex;    // Queue Family Index that supports Graphics
	std::vector<uint32_t>				mPresentQueueFamilyIndex;     // Queue Family Index that supports Present
	std::vector<vk::SurfaceFormatKHR>	mSurfaceFormats;			  // Surface formats supported by the Device
};

