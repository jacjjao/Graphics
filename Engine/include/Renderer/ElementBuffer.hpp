#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

namespace Engine
{

    class ElementBuffer
    {
    public:
        explicit ElementBuffer(size_t size);
        ~ElementBuffer();

        ElementBuffer(const ElementBuffer&) = delete;
        ElementBuffer& operator=(const ElementBuffer&) = delete;

        ElementBuffer(ElementBuffer&& other) noexcept;
        ElementBuffer& operator=(ElementBuffer&& other) noexcept;

        ElementBuffer& operator=(std::vector<uint32_t> indices);
        ElementBuffer& operator=(std::initializer_list<uint32_t> list);

        void update();

        void destroy();

        [[nodiscard]] 
        size_t size() const { return m_indices.size(); }

        static void bind(ElementBuffer* ebo);
        static void unbind();

    private:
        static uint32_t ebo_in_bind;

        std::vector<uint32_t> m_indices;
        uint32_t              m_id;
    };

} // namespace Engine