#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

class SwapChain
{
private:

public:
    SwapChain(vk::Instance& instance, vk::SurfaceKHR& surface);
    ~SwapChain();
};