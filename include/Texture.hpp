#pragma once


#include <cstdint>
#include <string>

class Texture
{
public:
    explicit Texture(const std::string& path) noexcept;
    ~Texture() noexcept;

    void destroy() noexcept;

    [[nodiscard]] int32_t getWidth() const noexcept;
    [[nodiscard]] int32_t getHeight() const noexcept;

    static void bind(const Texture&) noexcept;
    static void unbind() noexcept;

private:
    uint32_t m_id;
    int32_t  m_width, m_height;
};