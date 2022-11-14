#include "../include/ShaderProgram.hpp"
#include "../include/glCheck.hpp"

#include <fstream>
#include <sstream>

#include <fmt/core.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(const char* vertex_path, const char* fragment_path) : m_id(0)
{
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);
        std::stringstream v_shader_stream{};
        std::stringstream f_shader_stream{};

        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        v_shader_file.close();
        f_shader_file.close();

        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        fmt::print(stderr, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n{}\n", e.what());
    }

    const auto* v_shader_code = vertex_code.c_str();
    const auto* f_shader_code = fragment_code.c_str();

    const auto checkShaderProgramCompileStatus = [](const GLuint& shader, const char* name) {
        int32_t success = 0;
        char info_log[512] = {};
        glCheck(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            glCheck(glGetShaderInfoLog(shader, 512, nullptr, info_log));
            fmt::print(stderr, "ERROR::{}_SHADER::COMPILE\n{}\n", name, info_log);
        }
    };

    const auto checkProgramLinkStatus = [](const GLuint& program) {
        int32_t success = 0;
        char info_log[512] = {};
        glCheck(glGetProgramiv(program, GL_LINK_STATUS, &success));
        if (!success)
        {
            glCheck(glGetProgramInfoLog(program, 512, nullptr, info_log));
            fmt::print(stderr, "ERROR::PROGRAM::LINK\n{}\n", info_log);
        }
    };

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glCheck(glShaderSource(vertex, 1, &v_shader_code, nullptr));
    glCheck(glCompileShader(vertex));
    checkShaderProgramCompileStatus(vertex, "VECTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glCheck(glShaderSource(fragment, 1, &f_shader_code, nullptr));
    glCheck(glCompileShader(fragment));
    checkShaderProgramCompileStatus(fragment, "FRAGMENT");

    m_id = glCreateProgram();
    glCheck(glAttachShader(m_id, vertex));
    glCheck(glAttachShader(m_id, fragment));
    glCheck(glLinkProgram(m_id));
    checkProgramLinkStatus(m_id);

    glCheck(glDeleteShader(vertex));
    glCheck(glDeleteShader(fragment));
}

ShaderProgram::~ShaderProgram()
{
    destroy();
}

void ShaderProgram::use() const
{
    glCheck(glUseProgram(m_id));
}

void ShaderProgram::destroy()
{
    if (m_id > 0)
    {
        glCheck(glDeleteProgram(m_id));
        m_id = 0;
    }
}

uint32_t ShaderProgram::getID() const noexcept
{
    return m_id;
}

void ShaderProgram::setI(const char* name, const int32_t value) const
{
    GLint location = glGetUniformLocation(m_id, name);
    glCheck(glUniform1i(location, value));
}

void ShaderProgram::setF(const char* name, const float value) const
{
    GLint location = glGetUniformLocation(m_id, name);
    glCheck(glUniform1f(location, value));
}

void ShaderProgram::setMat4(const char* name, const glm::mat4 matrix) const
{
    GLint location = glGetUniformLocation(m_id, name);
    glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void ShaderProgram::setVec3(const char* name, const float x, const float y, const float z) const
{
    GLint location = glGetUniformLocation(m_id, name);
    glCheck(glUniform3f(location, x, y, z));
}

void ShaderProgram::setVec3(const char* name, const glm::vec3 vec) const
{
    GLint location = glGetUniformLocation(m_id, name);
    glCheck(glUniform3f(location, vec.x, vec.y, vec.z));
}
