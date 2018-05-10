#include "PipelineLayout.h"
// includes
#include "DevConstants.h"
#include "Device.h"
#include "DescriptorSetLayout.h"
#include "Error.h"

PipelineLayout::PipelineLayout(
    Device			            &device,
    const uint32_t              layoutCount,
    const DescriptorSetLayout*  layouts)
    : mDevice(&device)
{
    vk::PipelineLayoutCreateInfo info;
    info.pNext = nullptr;
    info.pushConstantRangeCount = 0;
    info.pPushConstantRanges = nullptr;
    info.setLayoutCount = 1;
    info.pSetLayouts = layouts->GetLayouts();
    
    // creaitng one pipeline layout here only
    mLayouts.resize(1);
    WERROR(device.GetDevice().createPipelineLayout(&info, nullptr, &mLayouts[0]),
        "Failed to create pipeline layout.");
}

PipelineLayout::~PipelineLayout()
{
    for (size_t i = 0; i < 1; ++i)
    {
        mDevice->GetDevice().destroyPipelineLayout(mLayouts[i], nullptr);
    }
}
