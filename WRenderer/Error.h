#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>

#ifdef _DEBUG
#define WASSERT(a, b)\
				{\
					assert(a);\
                    if(!a)\
                    {\
                        std::cout << b << std::endl;\
                    }\
				}

#define WERROR(a, b)\
                {\
                    assert(a == vk::Result::eSuccess);\
                    if(a != vk::Result::eSuccess)\
                    {\
                        std::cout << b << std::endl;\
                    }\
                }
#else
#define WASSERT(a, b) {}
#endif