#pragma once

#include <vulkan/vulkan.hpp>

#include "DevConstants.h"
#include "DevStructs.h"

class Texture
{
public:
	Texture(
		vk::Instance& instance, 
		Device& device, 
		uint32_t width, 
		uint32_t height);

	~Texture();
};