#pragma once

#include <cstdint>
#include <string>
#include <filesystem>
#include <unordered_map>

#include "include/Core/Matrix.hpp"
#include "include/Core/Vector.hpp"
#include "include/Core/Color.hpp"

namespace Engine
{

    static std::string read_shader_code(const std::filesystem::path& path);
    static bool checkShaderCompileStatus(uint32_t shader);
    static bool checkProgramLinkStatus(uint32_t program);

    class ShaderProgram
    {
    public:
        ShaderProgram() = default;
        explicit ShaderProgram(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path);
        ~ShaderProgram();

        void create(const std::string& vertex_src, const std::string& fragment_src);

        void use();
        static void unuse();

        [[nodiscard]] 
        uint32_t getID() const { return m_id; }

        void setI32(const std::string& name, int32_t value);
        void setFloat(const std::string& name, float value);
        void setBool(const std::string& name, bool value);
        void setMat3(const std::string& name, const Matrix3& matrix);
        void setMat4(const std::string& name, const Matrix4& matrix);
        void setVec3(const std::string& name, Vector3f vec);
        void setVec4(const std::string& name, Color color);

        void destroy();

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

    private:
        static uint32_t program_in_use;

        int32_t getLocation(const std::string& name);

        uint32_t m_id = 0;

        std::unordered_map<std::string, int32_t> locations;
    };

    class DefaultShaderProgram : public ShaderProgram
    {
    public:
        explicit DefaultShaderProgram();

        [[nodiscard]] static DefaultShaderProgram& instance();

        DefaultShaderProgram(const DefaultShaderProgram&) = delete;
        DefaultShaderProgram& operator=(const DefaultShaderProgram&) = delete;
    };

    class TextShaderProgram : public ShaderProgram
    {
    public:
        explicit TextShaderProgram();

        [[nodiscard]] static TextShaderProgram& instance();

        TextShaderProgram(const TextShaderProgram&) = delete;
        TextShaderProgram& operator=(const TextShaderProgram&) = delete;
    };

    namespace ShaderSrcs
    {

        namespace GraphicsRendering
        {
            inline const std::string vertex_shader_src = R"(
                #version 460 core

                layout (location = 0) in vec2 in_pos;
                layout (location = 1) in vec4 in_color; 
                layout (location = 2) in vec2 in_tex_coord;
                
                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 proj;
                
                out vec4 frag_color;
                out vec2 tex_coord;
                
                void main()
                {
                	gl_Position = proj * view * model * vec4(in_pos, 0.0, 1.0);
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
                uniform sampler2D textures[32];
                uniform int unit_index;
                
                void main()
                {	
                    switch(unit_index)
	                {
	                	case  0: color = mix(texture(textures[ 0], tex_coord), frag_color, color_alpha); break;
	                	case  1: color = mix(texture(textures[ 1], tex_coord), frag_color, color_alpha); break;
	                	case  2: color = mix(texture(textures[ 2], tex_coord), frag_color, color_alpha); break;
	                	case  3: color = mix(texture(textures[ 3], tex_coord), frag_color, color_alpha); break;
	                	case  4: color = mix(texture(textures[ 4], tex_coord), frag_color, color_alpha); break;
	                	case  5: color = mix(texture(textures[ 5], tex_coord), frag_color, color_alpha); break;
	                	case  6: color = mix(texture(textures[ 6], tex_coord), frag_color, color_alpha); break;
	                	case  7: color = mix(texture(textures[ 7], tex_coord), frag_color, color_alpha); break;
	                	case  8: color = mix(texture(textures[ 8], tex_coord), frag_color, color_alpha); break;
	                	case  9: color = mix(texture(textures[ 9], tex_coord), frag_color, color_alpha); break;
	                	case 10: color = mix(texture(textures[10], tex_coord), frag_color, color_alpha); break;
	                	case 11: color = mix(texture(textures[11], tex_coord), frag_color, color_alpha); break;
	                	case 12: color = mix(texture(textures[12], tex_coord), frag_color, color_alpha); break;
	                	case 13: color = mix(texture(textures[13], tex_coord), frag_color, color_alpha); break;
	                	case 14: color = mix(texture(textures[14], tex_coord), frag_color, color_alpha); break;
	                	case 15: color = mix(texture(textures[15], tex_coord), frag_color, color_alpha); break;
	                	case 16: color = mix(texture(textures[16], tex_coord), frag_color, color_alpha); break;
	                	case 17: color = mix(texture(textures[17], tex_coord), frag_color, color_alpha); break;
	                	case 18: color = mix(texture(textures[18], tex_coord), frag_color, color_alpha); break;
	                	case 19: color = mix(texture(textures[19], tex_coord), frag_color, color_alpha); break;
	                	case 20: color = mix(texture(textures[20], tex_coord), frag_color, color_alpha); break;
	                	case 21: color = mix(texture(textures[21], tex_coord), frag_color, color_alpha); break;
	                	case 22: color = mix(texture(textures[22], tex_coord), frag_color, color_alpha); break;
	                	case 23: color = mix(texture(textures[23], tex_coord), frag_color, color_alpha); break;
	                	case 24: color = mix(texture(textures[24], tex_coord), frag_color, color_alpha); break;
	                	case 25: color = mix(texture(textures[25], tex_coord), frag_color, color_alpha); break;
	                	case 26: color = mix(texture(textures[26], tex_coord), frag_color, color_alpha); break;
	                	case 27: color = mix(texture(textures[27], tex_coord), frag_color, color_alpha); break;
	                	case 28: color = mix(texture(textures[28], tex_coord), frag_color, color_alpha); break;
	                	case 29: color = mix(texture(textures[29], tex_coord), frag_color, color_alpha); break;
	                	case 30: color = mix(texture(textures[30], tex_coord), frag_color, color_alpha); break;
	                	case 31: color = mix(texture(textures[31], tex_coord), frag_color, color_alpha); break; 
                    }
                }
            )";

        } // namespace GraphicsRendering

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
        } // namespace TextRendering


    } // namespace ShaderSrcs

} // namespace Engine