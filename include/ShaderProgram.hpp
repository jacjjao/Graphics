#pragma once

#include <cstdint>
#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class ShaderProgram
{
public:
    ShaderProgram(const char* vertex_path, const char* fragment_path);
    ~ShaderProgram();

    void use() const;

    [[nodiscard]] uint32_t getID() const noexcept;

    void setI(const char* name, int32_t value) const;
    void setF(const char* name, float value) const;
    void setMat4(const char* name, glm::mat4 matrix) const;
    void setVec3(const char* name, float x, float y, float z) const;
    void setVec3(const char* name, glm::vec3 vec) const;

    void destroy();

    ShaderProgram(ShaderProgram&) = delete;
    ShaderProgram& operator=(ShaderProgram&) = delete;

private:
    uint32_t m_id;
};