#include "SwapChain.h"
#include "Error.h"

SwapChain::SwapChain(
    vk::Instance& instance, 
    vk::SurfaceKHR& surface, 
    Device& device)
{
    // Get Surface Capabilities for the device
    vk::SurfaceCapabilitiesKHR surfaceCapabilities;
    WERROR(device.mPhysicalDevice.getSurfaceCapabilitiesKHR(surface, &surfaceCapabilities),
        "Failed to get surface capabilities.");

    // get details of what is supported by Surface Capabilities
    uint32_t swapChainImagesCount = surfaceCapabilities.minImageCount;
    uint32_t swapChainWidth = surfaceCapabilities.currentExtent.width;
    uint32_t swapChainHeight = surfaceCapabilities.currentExtent.height;
    vk::Extent2D extent = surfaceCapabilities.currentExtent;
    bool identity = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
                    == vk::SurfaceTransformFlagBitsKHR::eIdentity;

    // identify supported alpha bit
    vk::CompositeAlphaFlagBitsKHR alphaBit = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    alphaBit = GetSupportedAlphaBit(surfaceCapabilities);

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.setPNext(nullptr)
        .setCompositeAlpha(alphaBit)
        .setSurface(surface)
        .setImageArrayLayers(1)
        .setImageColorSpace(device.mSurfaceFormats[0].colorSpace)
        .setImageExtent(extent)
        .setImageFormat(device.mSurfaceFormats[0].format)
        .setMinImageCount(swapChainImagesCount)
        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
        .setPresentMode(vk::PresentModeKHR::eFifo)
        .setPreTransform(identity ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform)
        .setPQueueFamilyIndices(DEFAULT_FAMILY_INDEX)
        .setPQueueFamilyIndices(nullptr);
    WASSERT(device.mPresentQueueFamilyIndex[0] == device.mGraphicsQueueFamilyIndex[0], 
        "Unhandled different Queue Family Index.");

    vk::SwapchainKHR swapChain;
    WERROR(device.mDevice.createSwapchainKHR(&createInfo, nullptr, &swapChain),
        "Failed to create swap chain.");
}


SwapChain::~SwapChain()
{

}

vk::CompositeAlphaFlagBitsKHR SwapChain::GetSupportedAlphaBit(
    vk::SurfaceCapabilitiesKHR& surfaceCapabilities)
{
    vk::CompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = 
    {
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
        vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
        vk::CompositeAlphaFlagBitsKHR::eInherit
    };

    for (uint32_t i = 0; i < sizeof(compositeAlphaFlags); ++i)
    {
        if (compositeAlphaFlags[i] & surfaceCapabilities.supportedCompositeAlpha)
        {
            return compositeAlphaFlags[i];
        }
    }
    return vk::CompositeAlphaFlagBitsKHR::eOpaque;
}