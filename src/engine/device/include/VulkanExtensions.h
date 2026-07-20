#pragma once

#include <set>
#include <string>
#include <vector>

// Standalone functions to manage extension configuration

std::vector<const char *> getExtensions();

const std::vector<const char *> getRequiredPhysicalDeviceExtensions();