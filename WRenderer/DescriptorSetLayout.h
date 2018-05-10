#pragma once

// libraries
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

class Device;
class DescriptorSetLayout
{
public:
    DescriptorSetLayout(
        Device                     &device,
        const uint32_t             descriptorCount,
        const vk::DescriptorType   descriptorType,
        const vk::ShaderStageFlags shaderStageFlags);
    ~DescriptorSetLayout();

    const vk::DescriptorSetLayout* GetLayouts() const { return &mLayouts[0]; }

private:
    std::vector<vk::DescriptorSetLayout> mLayouts;
};