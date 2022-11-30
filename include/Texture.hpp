#pragma once

#include "Vector.hpp"

#include <cstdint>
#include <string>

class Texture
{
public:
    explicit Texture(const std::string& path) noexcept;
    ~Texture() noexcept;

    void destroy() noexcept;

    [[nodiscard]] float getWidth() const noexcept;
    [[nodiscard]] float getHeight() const noexcept;

    static void bind(Texture*) noexcept;
    static void unbind() noexcept;

private:
    static Texture* texture_in_bind;

    uint32_t m_id;
    Vector2f m_size;
};