#include "Texture.h"

Texture::Texture(
	vk::Instance& instance, 
	Device& device, 
	uint32_t width, 
	uint32_t height)
{
	vk::ImageCreateInfo createInfo;
    createInfo.setPNext(nullptr)
        .setImageType(vk::ImageType::e2D)
        .setFormat(vk::Format::eD16Unorm)
        .setExtent(vk::Extent3D(width, height, 1))
        .setMipLevels(1)
        .setArrayLayers(1)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment)
        .setQueueFamilyIndexCount(0)
        .setPQueueFamilyIndices(nullptr)
        .setSharingMode(vk::SharingMode::eExclusive);
}

Texture::~Texture()
{
	 
}