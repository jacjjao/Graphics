#pragma once

#include <filesystem>

class FileSystem
{
public:
    static std::filesystem::path getPath(const std::filesystem::path& path);
};