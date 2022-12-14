#pragma once

#include <cstdint>
#include <string>
#include <filesystem>
#include <unordered_map>

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Color.hpp"

class ShaderProgram
{
public:
    ShaderProgram(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path) noexcept;
    ~ShaderProgram() noexcept;

    void use() noexcept;

    [[nodiscard]] uint32_t getID() const noexcept;

    void setI32(const std::string& name, int32_t value) noexcept;
    void setFloat(const std::string& name, float value) noexcept;
    void setBool(const std::string& name, bool value) noexcept;
    void setMat3(const std::string& name, const Matrix3& matrix) noexcept;
    void setMat4(const std::string& name, const Matrix4& matrix) noexcept;
    void setVec3(const std::string& name, Vector3<float> vec) noexcept;
    void setVec4(const std::string& name, Color color) noexcept;

    void destroy() noexcept;

    ShaderProgram(const ShaderProgram&)            = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

private:
    static ShaderProgram* program_in_use;

    int32_t getLocation(const std::string& name) noexcept;

    uint32_t m_id;

    std::unordered_map<std::string, int32_t> locations;
};

class ShaderProgram2D : public ShaderProgram
{
public:
    ShaderProgram2D() noexcept;

    [[nodiscard]] static ShaderProgram2D& instance() noexcept;

    ShaderProgram2D(const ShaderProgram2D&)            = delete;
    ShaderProgram2D& operator=(const ShaderProgram2D&) = delete;
};

class TextShaderProgram : public ShaderProgram
{
public:
    TextShaderProgram() noexcept;

    [[nodiscard]] static TextShaderProgram& instance() noexcept;

    TextShaderProgram(const TextShaderProgram&)            = delete;
    TextShaderProgram& operator=(const TextShaderProgram&) = delete;
};