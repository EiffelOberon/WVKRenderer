#include "SwapChain.h"

#include "Error.h"

SwapChain::SwapChain(
    vk::Instance& instance, 
    vk::SurfaceKHR& surface, 
    Device& device)
{
    // Get Surface Capabilities for the device
    vk::SurfaceCapabilitiesKHR surfaceCapabilities;
    vk::Result res = device.mPhysicalDevice.getSurfaceCapabilitiesKHR(surface, &surfaceCapabilities);
    WASSERT(res == vk::Result::eSuccess, "Failed to get surface capabilities");

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
        .setImageFormat(device.mSurfaceFormats[0].format)
        .setMinImageCount(swapChainImagesCount)
        .setImageExtent(extent)
        .setPresentMode(vk::PresentModeKHR::eFifo)
        .setPreTransform(identity ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform);

    
    /*createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.hinstance = info.connection;
    createInfo.hwnd = info.window;
    res = vkCreateWin32SurfaceKHR(info.inst, &createInfo, NULL, &info.surface);*/
}

SwapChain::~SwapChain()
{

}

vk::CompositeAlphaFlagBitsKHR GetSupportedAlphaBit(
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