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

public:
    SwapChain(vk::Instance& instance, vk::SurfaceKHR& surface, Device& device);
    ~SwapChain();
};