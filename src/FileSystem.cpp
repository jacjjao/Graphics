#include "../include/FileSystem.hpp"
#include "../configuration/root_directory.h"

std::string FileSystem::getPath(const std::string& path)
{
    return std::string(ROOT_DIR) + path;
}