#pragma once

#include <string>
#include "../configuration/root_directory.h"

class FileSystem
{
public:
    static std::string getPath(const std::string& path)
    {
        return std::string(ROOT_DIR) + path;
    }
};