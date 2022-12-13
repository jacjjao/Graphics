#include "../include/FileSystem.hpp"

#include "../configuration/root_directory.h"

std::filesystem::path FileSystem::getPath(const std::filesystem::path& path) noexcept
{
    std::filesystem::path target_path{ROOT_DIR};
    target_path += path;
    return target_path;
}