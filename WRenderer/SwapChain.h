#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

#include "Device.h"
#include "DevConstants.h"
#include "DevStructs.h"
#include "DepthTexture.h"

class DeviceInfo;
class SwapChain
{
public:
	SwapChain(
		vk::Instance	&instance,
		vk::SurfaceKHR	&surface,
		Device          &device);

	~SwapChain();

private:
	void CreateBackBuffers(
		Device				&device);

    vk::CompositeAlphaFlagBitsKHR GetSupportedAlphaBit(
		vk::SurfaceCapabilitiesKHR       &surfaceCapabilities);

	// vulkan objects
    vk::SwapchainKHR		mSwapChain;

	// logical objects for render pipeline
    std::vector<BackBuffer>			mBackBuffers;
	std::unique_ptr<DepthTexture>	mDepthBuffer;
};