#include "DescriptorSetLayout.h"

// includes
#include "DevConstants.h"
#include "Device.h"
#include "Error.h"
#include <vector>

DescriptorSetLayout::DescriptorSetLayout(
    Device                     &device,
    const uint32_t             descriptorCount,
    const vk::DescriptorType   descriptorType,
    const vk::ShaderStageFlags shaderStageFlags)
{
    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    bindings.resize(descriptorCount);
    for (size_t i = 0; i < descriptorCount; ++i)
    {
        bindings[i].binding = 0;
        bindings[i].descriptorType = descriptorType;
        bindings[i].descriptorCount = descriptorCount;
        bindings[i].stageFlags = shaderStageFlags;
        bindings[i].setPImmutableSamplers(nullptr);
    }

    vk::DescriptorSetLayoutCreateInfo info;
    info.setPNext(nullptr);
    info.bindingCount = descriptorCount;
    info.pBindings = &bindings[0];
    
    // creating one descriptor set layout
    mLayouts.resize(1);
    WERROR(device.GetDevice().createDescriptorSetLayout(&info, nullptr, &mLayouts[0]),
        "Failed to create descriptor set layout.");
}

DescriptorSetLayout::~DescriptorSetLayout()
{

}
