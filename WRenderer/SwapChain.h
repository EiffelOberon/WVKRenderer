#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

#include "DevConstants.h"
#include "DevStructs.h"

class SwapChain
{
private:
    vk::CompositeAlphaFlagBitsKHR GetSupportedAlphaBit(vk::SurfaceCapabilitiesKHR& surfaceCapabilities);
    void InitBackBuffers(Device& device);

    vk::SwapchainKHR mSwapChain;
    std::vector<BackBuffer> mBackBuffers;
public:
    SwapChain(vk::Instance& instance, vk::SurfaceKHR& surface, Device& device);
    ~SwapChain();
};