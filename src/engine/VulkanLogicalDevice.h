#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanPhysicalDevice.h"

class VulkanLogicalDevice
{
public:
    VulkanLogicalDevice(const std::shared_ptr<VulkanPhysicalDevice> physicalDevice);
    ~VulkanLogicalDevice();

private:
};