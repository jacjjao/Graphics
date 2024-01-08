#include "pch.hpp"
#include "include/Renderer/Circle2D.hpp"
#include "include/Renderer/ShaderProgram.hpp"

namespace eg
{

    Circle2D::Circle2D(const float radius, const size_t point_count) :
        Shape{ point_count + 2 },
        m_radius{ radius }
    {
        update();
    }

    void Circle2D::draw()
    {
        m_vao.draw(
            PrimitiveType::TriangleFan,
            getTransformMatrix(),
            hasTexture() ? 0.0F : 1.0F,
            m_texture.get()
        );
    }

    void Circle2D::update()
    {
        constexpr auto pi = std::numbers::pi_v<float>;

        const auto f_point_count = static_cast<float>(getPointCount());
        const auto slice = 2.0F * pi / f_point_count;
        const auto tex_center = m_tex_rect.position + m_tex_rect.size * 0.5f;
        const auto half_tex_size = m_tex_rect.size * 0.5f;
        const auto center = getPosition();

        setOrigin(center);

        m_vao[0].position = center;
        m_vao[0].color = m_color;
        m_vao[0].tex_coord = tex_center;
        for (size_t i = 1; i < m_vao.size() - 1; i++)
        {
            const auto theta = -slice * static_cast<float>(i - 1) + (pi / 2.0f); // start from 90 degree, clockwise
            const auto ssin = std::sin(theta);
            const auto ccos = std::cos(theta);

            m_vao[i].position = {
                center.x + m_radius * ccos,
                center.y + m_radius * ssin
            };
            m_vao[i].color = m_color;
            if (hasTexture())
            {
                Vector2f tex_coord{};

                tex_coord.x = tex_center.x + half_tex_size.x * ccos;
                tex_coord.y = tex_center.y + half_tex_size.y * ssin;

                m_vao[i].tex_coord = tex_coord;
            }
        }
        m_vao.back() = m_vao[1];

        m_vao.update();
    }

    Vector2f Circle2D::getPoint(const size_t index)
    {
        return Shape::getPoint(index + 1);
    }

    void Circle2D::getAllTransformPoint(std::vector<Vector2f>& container, [[maybe_unused]] size_t from) const
    {
        return Shape::getAllTransformPoint(container, 1);
    }

} // namespace eg