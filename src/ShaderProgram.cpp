#include "../include/pch.hpp"
#include "../include/ShaderProgram.hpp"

#include <glad/glad.h>

#include <fstream>
#include <stdexcept>

#define SUCCESS true
#define FAILED false

std::string read_shader_code(const std::filesystem::path & path)
{
    std::ifstream file{};

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    file.open(path);
    if (!file.is_open())
    {
        throw std::runtime_error{std::string{"Cannot open the file: "} + path.string()};
    }

    std::stringstream code{};
    code << file.rdbuf();

    return std::move(code).str();
}

bool checkShaderCompileStatus(const uint32_t shader) noexcept
{
    int32_t success       = 0;
    char    info_log[512] = {};
    glCheck(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        glCheck(glGetShaderInfoLog(shader, 512, nullptr, info_log));
        std::cerr << "ERROR::SHADER::COMPILE\n" << info_log << '\n';
        return FAILED;
    }
    return SUCCESS;
}

bool checkProgramLinkStatus(const uint32_t program) noexcept
{
    int32_t success       = 0;
    char    info_log[512] = {};
    glCheck(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (!success)
    {
        glCheck(glGetProgramInfoLog(program, 512, nullptr, info_log));
        std::cerr << "ERROR::PROGRAM::LINK\n" << info_log << '\n';
        return FAILED;
    }
    return SUCCESS;
}

ShaderProgram* ShaderProgram::program_in_use = nullptr;

ShaderProgram::ShaderProgram(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path) noexcept
{
    std::string   vertex_code{};
    std::string   fragment_code{};
    
    try
    {
        vertex_code = read_shader_code(vertex_path);
        fragment_code = read_shader_code(fragment_path);
    } catch (const std::exception& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << e.what() << '\n';
    }

    create(vertex_code, fragment_code);
}

ShaderProgram::~ShaderProgram() noexcept
{
    destroy();
}

void ShaderProgram::create(const std::string& vertex_src, const std::string& fragment_src) noexcept
{
    const auto* v_shader_code = vertex_src.data();
    const auto* f_shader_code = fragment_src.data();

    GLuint vertex = 0;
    glCheck(vertex = glCreateShader(GL_VERTEX_SHADER));
    glCheck(glShaderSource(vertex, 1, &v_shader_code, nullptr));
    glCheck(glCompileShader(vertex));
    std::cout << "Checking vertex shader's compile status...\n";
    if (checkShaderCompileStatus(vertex))
    {
        std::cout << "Vertex shader has been successfully compiled\n";
    }

    GLuint fragment = 0;
    glCheck(fragment = glCreateShader(GL_FRAGMENT_SHADER));
    glCheck(glShaderSource(fragment, 1, &f_shader_code, nullptr));
    glCheck(glCompileShader(fragment));
    std::cout << "Checking fragment shader's compile status...\n";
    if (checkShaderCompileStatus(fragment))
    {
        std::cout << "Fragment shader has been successfully compiled\n";
    }

    glCheck(m_id = glCreateProgram());
    glCheck(glAttachShader(m_id, vertex));
    glCheck(glAttachShader(m_id, fragment));
    glCheck(glLinkProgram(m_id));
    std::cout << "Checking shader program's link status...\n";
    if (checkProgramLinkStatus(m_id))
    {
        std::cout << "Shaders have been successfully linked to the program\n";
    }

    glCheck(glDeleteShader(vertex));
    glCheck(glDeleteShader(fragment));
}

void ShaderProgram::use() noexcept
{
    if (program_in_use != this)
    {
        glCheck(glUseProgram(m_id));
        program_in_use = this;
    }
}

void ShaderProgram::unuse() noexcept
{
    if (program_in_use != nullptr)
    {
        glCheck(glUseProgram(0));
        program_in_use = nullptr;
    }
}

uint32_t ShaderProgram::getID() const noexcept
{
    return m_id;
}

void ShaderProgram::setI32(const std::string& name, const int32_t value) noexcept
{
    const auto loc = getLocation(name);
    glCheck(glUniform1i(loc, value));
}

void ShaderProgram::setFloat(const std::string& name, const float value) noexcept
{
    const auto loc = getLocation(name);
    glCheck(glUniform1f(loc, value));
}

void ShaderProgram::setBool(const std::string& name, const bool value) noexcept
{
    const auto loc = getLocation(name);
    glCheck(glUniform1i(loc, value));
}

void ShaderProgram::setMat4(const std::string& name, const Matrix4& matrix) noexcept
{
    const auto loc = getLocation(name);
    glCheck(glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.data()));
}

void ShaderProgram::setVec3(const std::string& name, const Vector3f vec) noexcept
{
    const auto loc = getLocation(name);
    glCheck(glUniform3f(loc, vec.x, vec.y, vec.z));
}

void ShaderProgram::setVec4(const std::string& name, const Color color) noexcept
{
    const auto loc = getLocation(name);

    const auto r = static_cast<float>(color.r) / 255.0F;
    const auto g = static_cast<float>(color.g) / 255.0F;
    const auto b = static_cast<float>(color.b) / 255.0F;
    const auto a = static_cast<float>(color.a) / 255.0F;

    glCheck(glUniform4f(loc, r, g, b, a));
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
    const auto it = locations.find(name);

    GLint loc = 0;

    const bool is_found = (it != locations.end());
    if (!is_found)
    {
        glCheck(loc = glGetUniformLocation(m_id, name.c_str()));
        if (loc != -1)
        {
            locations[name] = loc;
        }
        else
        {
            std::cerr << "Cannot found uniform " << name << '\n';
        }
    }
    else
    {
        loc = it->second;
    }
    return loc;
}

DefaultShaderProgram::DefaultShaderProgram() noexcept :
ShaderProgram{}
{
    create(ShaderSrcs::GraphicsRendering::vertex_shader_src, ShaderSrcs::GraphicsRendering::fragment_shader_src);
}

DefaultShaderProgram& DefaultShaderProgram::instance() noexcept
{
    static DefaultShaderProgram program{};
    return program;
}

TextShaderProgram::TextShaderProgram() noexcept : ShaderProgram{}
{
    create(ShaderSrcs::TextRendering::vertex_shader_src, ShaderSrcs::TextRendering::fragment_shader_src);
}

TextShaderProgram& TextShaderProgram::instance() noexcept
{
    static TextShaderProgram program{};
    return program;
}