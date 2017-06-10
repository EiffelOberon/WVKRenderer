#include "SwapChain.h"

SwapChain::SwapChain(vk::Instance& instance, vk::SurfaceKHR& surface)
{
    /*createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.hinstance = info.connection;
    createInfo.hwnd = info.window;
    res = vkCreateWin32SurfaceKHR(info.inst, &createInfo, NULL, &info.surface);*/
}

SwapChain::~SwapChain()
{

}