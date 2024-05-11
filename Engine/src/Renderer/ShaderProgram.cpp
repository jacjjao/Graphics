#include "pch.hpp"
#include "include/Renderer/ShaderProgram.hpp"
#include "include/Core/Log.hpp"


namespace eg
{

    std::string read_shader_code(const std::filesystem::path& path)
    {
        std::ifstream file{};

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        file.open(path);
        if (!file.is_open())
        {
            throw std::runtime_error{ std::string{"Cannot open the file: "} + path.string() };
        }

        std::stringstream code{};
        code << file.rdbuf();

        return std::move(code).str();
    }

    bool checkShaderCompileStatus(const uint32_t shader)
    {
        int32_t success = 0;
        char    info_log[512] = {};
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, nullptr, info_log);
            EG_CORE_ERROR("Shader compile error!\n{}", info_log);
            return false;
        }
        return true;
    }

    bool checkProgramLinkStatus(const uint32_t program)
    {
        int32_t success = 0;
        char    info_log[512] = {};
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 512, nullptr, info_log);
            EG_CORE_ERROR("Shader program link error!\n{}", info_log);
            return false;
        }
        return true;
    }

    uint32_t ShaderProgram::program_in_use = 0;

    ShaderProgram::ShaderProgram(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path)
    {
        std::string   vertex_code{};
        std::string   fragment_code{};

        vertex_code = read_shader_code(vertex_path);
        fragment_code = read_shader_code(fragment_path);

        create(vertex_code, fragment_code);
    }

    ShaderProgram::~ShaderProgram()
    {
        destroy();
    }

    void ShaderProgram::create(const std::string& vertex_src, const std::string& fragment_src)
    {
        const auto* v_shader_code = vertex_src.data();
        const auto* f_shader_code = fragment_src.data();

        GLuint vertex = 0;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, nullptr);
        glCompileShader(vertex);
        if (checkShaderCompileStatus(vertex))
        {
            EG_CORE_INFO("Vertex shader has been successfully compiled");
        }

        GLuint fragment = 0;
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, nullptr);
        glCompileShader(fragment);
        if (checkShaderCompileStatus(fragment))
        {
            EG_CORE_INFO("Fragment shader has been successfully compiled");
        }

        m_id = glCreateProgram();
        glAttachShader(m_id, vertex);
        glAttachShader(m_id, fragment);
        glLinkProgram(m_id);
        if (checkProgramLinkStatus(m_id))
        {
            EG_CORE_INFO("Shaders have been successfully linked to the program");
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void ShaderProgram::use() const
    {
        if (program_in_use != m_id)
        {
            glUseProgram(m_id);
            program_in_use = m_id;
        }
    }

    void ShaderProgram::unuse()
    {
        if (program_in_use != 0)
        {
            glUseProgram(0);
            program_in_use = 0;
        }
    }

    void ShaderProgram::setI32(const std::string& name, const int32_t value)
    {
        const auto loc = getLocation(name);
        glUniform1i(loc, value);
    }

    void ShaderProgram::setFloat(const std::string& name, const float value)
    {
        const auto loc = getLocation(name);
        glUniform1f(loc, value);
    }

    void ShaderProgram::setBool(const std::string& name, const bool value)
    {
        const auto loc = getLocation(name);
        glUniform1i(loc, value);
    }

    void ShaderProgram::setMat3(const std::string& name, const Matrix3& matrix)
    {
        const auto loc = getLocation(name);
        glUniformMatrix3fv(loc, 1, GL_TRUE, matrix.data());
    }
    
    void ShaderProgram::setMat4(const std::string& name, const Matrix4& matrix)
    {
        const auto loc = getLocation(name);
        glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.data());
    }
    
    void ShaderProgram::setVec3(const std::string& name, const Vector3f vec)
    {
        const auto loc = getLocation(name);
        glUniform3f(loc, vec.x, vec.y, vec.z);
    }

    void ShaderProgram::setVec4(const std::string& name, const Color color)
    {
        const auto loc = getLocation(name);

        const auto r = static_cast<float>(color.r) / 255.0F;
        const auto g = static_cast<float>(color.g) / 255.0F;
        const auto b = static_cast<float>(color.b) / 255.0F;
        const auto a = static_cast<float>(color.a) / 255.0F;

        glUniform4f(loc, r, g, b, a);
    }

    void ShaderProgram::destroy()
    {
        if (m_id > 0)
        {
            glDeleteProgram(m_id);
            m_id = 0;
        }
    }

    int32_t ShaderProgram::getLocation(const std::string& name)
    {
        const auto it       = locations.find(name);
        const auto is_found = (it != locations.end());
        if (is_found)
        {
            return it->second;
        }

        const auto loc            = glGetUniformLocation(m_id, name.c_str());
        const auto uniform_exists = (loc != -1);
        if (uniform_exists)
        {
            locations[name] = loc;
        }
        else
        {
            EG_CORE_ERROR("Cannot found uniform {}", name);
        }
        return loc;
    }

    DefaultShaderProgram::DefaultShaderProgram() :
        ShaderProgram{}
    {
        create(ShaderSrcs::GraphicsRendering::vertex_shader_src, ShaderSrcs::GraphicsRendering::fragment_shader_src);
    }

    DefaultShaderProgram& DefaultShaderProgram::instance()
    {
        static DefaultShaderProgram program{};
        return program;
    }

    TextShaderProgram::TextShaderProgram() : ShaderProgram{}
    {
        create(ShaderSrcs::TextRendering::vertex_shader_src, ShaderSrcs::TextRendering::fragment_shader_src);
    }

    TextShaderProgram& TextShaderProgram::instance()
    {
        static TextShaderProgram program{};
        return program;
    }

} // namespace eg