#pragma once

// libraries
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

class Device;
class DescriptorSetLayout;
class PipelineLayout
{
public:
    PipelineLayout(
        Device			            &device,
        const uint32_t              layoutCount,
        const DescriptorSetLayout*  layouts);
    ~PipelineLayout();

private:
    // vulkan objects
    std::vector<vk::PipelineLayout> mLayouts;

    // weak pointer 
    Device                          *mDevice;
};