#include "../include/ShaderProgram.hpp"

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/FileSystem.hpp"
#include "../include/glCheck.hpp"

ShaderProgram::ShaderProgram(const char* vertex_path, const char* fragment_path) noexcept : m_id(0)
{
    std::string   vertex_code;
    std::string   fragment_code;
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

        vertex_code   = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    } catch (const std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << '\n';
    }

    const auto* v_shader_code = vertex_code.c_str();
    const auto* f_shader_code = fragment_code.c_str();

    const auto checkShaderProgramCompileStatus = [](const GLuint& shader, const char* name)
    {
        int32_t success       = 0;
        char    info_log[512] = {};
        glCheck(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            glCheck(glGetShaderInfoLog(shader, 512, nullptr, info_log));
            std::cerr << "ERROR::" << name << "_SHADER::COMPILE\n" << info_log << '\n';
        }
    };

    const auto checkProgramLinkStatus = [](const GLuint& program)
    {
        int32_t success       = 0;
        char    info_log[512] = {};
        glCheck(glGetProgramiv(program, GL_LINK_STATUS, &success));
        if (!success)
        {
            glCheck(glGetProgramInfoLog(program, 512, nullptr, info_log));
            std::cerr << "ERROR::PROGRAM::LINK\n" << info_log << '\n';
        }
    };

    GLuint vertex{};
    glCheck(vertex = glCreateShader(GL_VERTEX_SHADER));
    glCheck(glShaderSource(vertex, 1, &v_shader_code, nullptr));
    glCheck(glCompileShader(vertex));
    checkShaderProgramCompileStatus(vertex, "VECTEX");

    GLuint fragment{};
    glCheck(fragment = glCreateShader(GL_FRAGMENT_SHADER));
    glCheck(glShaderSource(fragment, 1, &f_shader_code, nullptr));
    glCheck(glCompileShader(fragment));
    checkShaderProgramCompileStatus(fragment, "FRAGMENT");

    glCheck(m_id = glCreateProgram());
    glCheck(glAttachShader(m_id, vertex));
    glCheck(glAttachShader(m_id, fragment));
    glCheck(glLinkProgram(m_id));
    checkProgramLinkStatus(m_id);

    glCheck(glDeleteShader(vertex));
    glCheck(glDeleteShader(fragment));
}

ShaderProgram::~ShaderProgram() noexcept
{
    destroy();
}

void ShaderProgram::use() const
{
    glCheck(glUseProgram(m_id));
}

uint32_t ShaderProgram::getID() const noexcept
{
    return m_id;
}

void ShaderProgram::setI32(const std::string& name, const int32_t value) noexcept
{
    auto loc = getLocation(name);
    glCheck(glUniform1i(loc, value));
}

void ShaderProgram::setFloat(const std::string& name, const float value) noexcept
{
    auto loc = getLocation(name);
    glCheck(glUniform1f(loc, value));
}

void ShaderProgram::setMat4(const std::string& name, const Matrix4<float>& matrix) noexcept
{
    auto loc = getLocation(name);
    glCheck(glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.data()));
}

void ShaderProgram::setVec3(const std::string& name, const float x, const float y, const float z) noexcept
{
    auto loc = getLocation(name);
    glCheck(glUniform3f(loc, x, y, z));
}

void ShaderProgram::setVec3(const std::string& name, const Vector3<float> vec) noexcept
{
    auto loc = getLocation(name);
    glCheck(glUniform3f(loc, vec.x, vec.y, vec.z));
}

void ShaderProgram::destroy() noexcept
{
    if (m_id > 0)
    {
        glCheck(glDeleteProgram(m_id));
        m_id = 0;
    }
}

int32_t ShaderProgram::getLocation(const std::string& name) noexcept
{
    auto       it       = locations.find(name);
    GLint      loc      = 0;
    const bool is_found = (it != locations.end());
    if (!is_found)
    {
        glCheck(loc = glGetUniformLocation(m_id, name.c_str()));
        locations[name] = loc;
    }
    else
    {
        loc = it->second;
    }
    return loc;
}

ShaderProgram2D::ShaderProgram2D() noexcept :
ShaderProgram{FileSystem::getPath("/shader/2D/VertexShader.vert").c_str(),
              FileSystem::getPath("/shader/2D/FragmentShader.frag").c_str()}
{
}

ShaderProgram2D& ShaderProgram2D::instance() noexcept
{
    static ShaderProgram2D program{};
    return program;
}