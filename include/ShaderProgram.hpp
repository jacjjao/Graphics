#pragma once

#include <cstdint>
#include <string>
#include <filesystem>
#include <unordered_map>

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Color.hpp"

static std::string read_shader_code(const std::filesystem::path& path);
static bool checkShaderCompileStatus(uint32_t shader) noexcept;
static bool checkProgramLinkStatus(uint32_t program) noexcept;

class ShaderProgram
{
public:
    ShaderProgram() = default;
    explicit ShaderProgram(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path) noexcept;
    ~ShaderProgram() noexcept;

    void create(const std::string& vertex_src, const std::string& fragment_src) noexcept;

    void use() noexcept;

    [[nodiscard]] uint32_t getID() const noexcept;

    void setI32(const std::string& name, int32_t value) noexcept;
    void setFloat(const std::string& name, float value) noexcept;
    void setBool(const std::string& name, bool value) noexcept;
    void setMat4(const std::string& name, const Matrix4& matrix) noexcept;
    void setVec3(const std::string& name, Vector3f vec) noexcept;
    void setVec4(const std::string& name, Color color) noexcept;

    void destroy() noexcept;

    ShaderProgram(const ShaderProgram&)            = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

private:
    static ShaderProgram* program_in_use;

    int32_t getLocation(const std::string& name) noexcept;

    uint32_t m_id = 0;

    std::unordered_map<std::string, int32_t> locations;
};

class DefaultShaderProgram : public ShaderProgram
{
public:
    explicit DefaultShaderProgram() noexcept;

    [[nodiscard]] static DefaultShaderProgram& instance() noexcept;

    DefaultShaderProgram(const DefaultShaderProgram&)            = delete;
    DefaultShaderProgram& operator=(const DefaultShaderProgram&) = delete;
};

class TextShaderProgram : public ShaderProgram
{
public:
    explicit TextShaderProgram() noexcept;

    [[nodiscard]] static TextShaderProgram& instance() noexcept;

    TextShaderProgram(const TextShaderProgram&)            = delete;
    TextShaderProgram& operator=(const TextShaderProgram&) = delete;
};

namespace ShaderSrcs 
{

    namespace GraphicsRendering 
    {
        inline const std::string vertex_shader_src = R"(
            #version 460 core

            layout (location = 0) in vec3 in_pos;
            layout (location = 1) in vec4 in_color; 
            layout (location = 2) in vec2 in_tex_coord;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 proj;
            
            out vec4 frag_color;
            out vec2 tex_coord;
            
            void main()
            {
            	gl_Position = proj * view * model * vec4(in_pos, 1.0);
            	frag_color = in_color;
            	tex_coord = in_tex_coord;
            }
        )"; 

        inline const std::string fragment_shader_src = R"(
            #version 460 core

            in vec4 frag_color;
            in vec2 tex_coord;
            
            out vec4 color;
            
            uniform float color_alpha;
            uniform sampler2D texture1;
            
            void main()
            {	
            	color = mix(texture(texture1, tex_coord), frag_color, color_alpha);
            }
        )";

    } // GraphicsRendering

    namespace TextRendering
    {
        inline const std::string vertex_shader_src = R"(
            #version 460 core
            layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
            out vec2 TexCoords;
            
            uniform mat4 proj;
            
            void main()
            {
                gl_Position = proj * vec4(vertex.xy, 0.0, 1.0);
                TexCoords = vertex.zw;
            }  
        )";

        inline const std::string fragment_shader_src = R"(
            #version 460 core
            in vec2 TexCoords;
            out vec4 color;
            
            uniform sampler2D text;
            uniform vec4 textColor;
            
            void main()
            {    
                vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
                color = textColor * sampled;
            }  
        )";
    } // TextRendering


} // ShaderSrcs