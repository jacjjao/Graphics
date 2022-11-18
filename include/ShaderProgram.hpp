#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <string>

class ShaderProgram
{
public:
    ShaderProgram(const char* vertex_path, const char* fragment_path);
    ~ShaderProgram();

    void use() const;

    [[nodiscard]] uint32_t getID() const noexcept;

    void setI32(const std::string& name, int32_t value);
    void setFloat(const std::string& name, float value);
    void setMat4(const std::string& name, glm::mat4 matrix);
    void setVec3(const std::string& name, float x, float y, float z);
    void setVec3(const std::string& name, glm::vec3 vec);

    void destroy();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

private:
    int32_t getLocation(const std::string& name);

    uint32_t m_id;
    std::unordered_map<std::string, int32_t> locations;
};

class ShaderProgram2D : public ShaderProgram
{
public:
    [[nodiscard]] static ShaderProgram2D& instance() noexcept;
    ShaderProgram2D();
};