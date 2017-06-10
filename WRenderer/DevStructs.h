#pragma once

/// Device object
struct Device
{
    vk::Device mDevice;                                 // Logical device object
    vk::PhysicalDevice mPhysicalDevice;                 // Physical device object
    vk::Bool32 mSupportPresent;                         // whether Present is supported
    std::vector<uint32_t> mGraphicsQueueFamilyIndex;    // Queue Family Index that supports Graphics
    std::vector<uint32_t> mPresentQueueFamilyIndex;     // Queue Family Index that supports Present
    std::vector<vk::SurfaceFormatKHR> mSurfaceFormats;  // Surface formats supported by the Device
};