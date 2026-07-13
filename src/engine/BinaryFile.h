#pragma once

#include <fstream>
#include <vector>

#include "Utilities.h"

class BinaryFile
{
public:
    BinaryFile(const std::string& path)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + path);
        }

        size_t size = static_cast<size_t>(file.tellg());

        m_data.resize(size);

        file.seekg(0);
        file.read(m_data.data(), size);
        file.close();
    }

    const std::vector<char>& getData() { return m_data; }

private:
    std::vector<char> m_data;
};