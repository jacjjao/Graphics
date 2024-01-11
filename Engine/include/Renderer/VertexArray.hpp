#pragma once

#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"
#include "include/Core/PrimitiveType.hpp"
#include "include/Core/Matrix.hpp"
#include "Texture.hpp"

#include <vector>

namespace eg
{

    struct BufferLayout
    {
        uint32_t index;
        int32_t component_count;
        uint32_t type;
        bool     normalize;
        int32_t  stride;
        uint64_t offset;
    };

    class VertexArray
    {
    private:
        using element_type   = Vertex;
        using container      = std::vector<Vertex>;
        using value_type     = container::value_type;
        using iterator       = container::iterator;
        using const_iterator = container::const_iterator;

    public:
        explicit VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) const = delete;

        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(VertexArray&& other) noexcept;

        void draw(size_t count,
                  PrimitiveType primitive_type = PrimitiveType::Triangles,
                  float color_alpha = 1.0F,
                  Texture* texture = nullptr) const;
        void drawIndices(int32_t size,
                         PrimitiveType primitive_type = PrimitiveType::Triangles,
                         float color_alpha = 1.0F,
                         Texture* texture = nullptr) const;

        void setElementBuffer(ElementBuffer& ebo) const;

        template<typename T>
        void addVertexBuffer(const VertexBuffer<T>& buffer, std::span<const BufferLayout> layouts) const
        {
            VertexArray::bind(*this);
            VertexBuffer<T>::bind(buffer);

            for (const auto& layout : layouts)
            {
                glVertexAttribPointer(layout.index,
                                      layout.component_count,
                                      layout.type,
                                      layout.normalize,
                                      layout.stride,
                                      (void*)layout.offset);
                glEnableVertexAttribArray(layout.index);
            }

            VertexArray::unbind();
            VertexBuffer<T>::unbind();
        }

        static void bind(const VertexArray& vao);
        static void unbind();

    private:
        void create();
        void destroy();

        static uint32_t vao_in_bind;

        uint32_t m_id;
    };

} // namespace eg
