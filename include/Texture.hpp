#pragma once

#include "Vector.hpp"

#include <cstdint>
#include <string>
class Texture
{
public:
    explicit Texture(const std::string& path) noexcept;
    ~Texture() noexcept;

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    void destroy() noexcept;

    [[nodiscard]] Vector2f getSize() const noexcept;

    [[nodiscard]] float getWidth() const noexcept;
    [[nodiscard]] float getHeight() const noexcept;

    static void bind(Texture*) noexcept;
    static void unbind() noexcept;

    static Vector2f pointToTexCoord(Vector2f point, Vector2f tex_size) noexcept;

private:
    static Texture* texture_in_bind;

    uint32_t m_id;
    Vector2f m_size;
};

class Image
{
public:
    explicit Image(const Texture& texture);

    [[nodiscard]] const Texture& getTexture() const noexcept;

    void setPosition(Vector2f position) noexcept;

    [[nodiscard]] Vector2f getPosition() const noexcept;

    void setSize(Vector2f size) noexcept;

    [[nodiscard]] Vector2f getSize() const noexcept;

private:
    const Texture& texture;

    Vector2f m_position;

    Vector2f m_size;
};