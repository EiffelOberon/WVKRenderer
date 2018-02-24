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
        .setPresentMode(vk::PresentModeKHR::eFifo)  //TODO: don't understand present mode
        .setPreTransform(identity ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform)  //TODO: don't understand swapchain transform
        .setPQueueFamilyIndices(DEFAULT_FAMILY_INDEX)
        .setPQueueFamilyIndices(nullptr);
    WASSERT(device.mPresentQueueFamilyIndex[0] == device.mGraphicsQueueFamilyIndex[0], 
        "Unhandled different Queue Family Index.");

    WERROR(device.mDevice.createSwapchainKHR(&createInfo, nullptr, &mSwapChain),
        "Failed to create swapchain.");

    InitBackBuffers(device);
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

void SwapChain::InitBackBuffers(Device& device)
{
    std::vector<vk::Image> images;
    std::vector<vk::ImageView> imageViews;

    WASSERT(mBackBuffers.size() == 0, "VKImage list must have no element at this point.");

    // verify image count for swapchain
    uint32_t count = 0;
    WERROR(device.mDevice.getSwapchainImagesKHR(mSwapChain, &count, nullptr),
        "Unable to retrieve swapchain images.");
    WASSERT((count > 0), "Swapchain image count is 0.");

    // get a list of mImages
    images.resize(count);
    imageViews.resize(count);
    mBackBuffers.resize(count);
    WERROR(device.mDevice.getSwapchainImagesKHR(mSwapChain, &count, &images[0]),
        "Unable to retrieve swapchain images.");

    // create image view
    for (uint32_t i = 0; i < count; ++i)
    {
        vk::ComponentMapping componentMapping(
            vk::ComponentSwizzle::eR,
            vk::ComponentSwizzle::eG,
            vk::ComponentSwizzle::eB,
            vk::ComponentSwizzle::eA);

        vk::ImageSubresourceRange range(
            vk::ImageAspectFlagBits::eColor, 
            0 /*base mip level*/, 
            1 /*level count*/, 
            0 /*base array layer*/, 
            1 /*layer count*/);

        vk::ImageViewCreateInfo createInfo;
        createInfo.setPNext(nullptr)
            .setImage(images[i])
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(device.mSurfaceFormats[0].format)
            .setComponents(componentMapping)
            .setSubresourceRange(range);

        WERROR(device.mDevice.createImageView(&createInfo, nullptr, &imageViews[i]),
            "Failed to create image view");

        mBackBuffers[i].mImage = images[i];
        mBackBuffers[i].mImageView = imageViews[i];
    }
}
