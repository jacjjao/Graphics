#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "Matrix.hpp"
#include "Vector.hpp"

class ShaderProgram
{
public:
    ShaderProgram(const char* vertex_path, const char* fragment_path) noexcept;
    ~ShaderProgram() noexcept;

    void use() noexcept;

    [[nodiscard]] uint32_t getID() const noexcept;

    void setI32(const std::string& name, int32_t value) noexcept;
    void setFloat(const std::string& name, float value) noexcept;
    void setBool(const std::string& name, bool value) noexcept;
    // void setMat3(const std::string& name, const Matrix3& matrix) noexcept;
    void setMat4(const std::string& name, const Matrix4& matrix) noexcept;
    void setVec3(const std::string& name, Vector3<float> vec) noexcept;

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