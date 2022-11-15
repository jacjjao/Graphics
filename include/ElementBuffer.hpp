#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

class ElementBuffer
{
public:
    explicit ElementBuffer(size_t size = 0);
    ~ElementBuffer();

    ElementBuffer(const ElementBuffer&) = delete;
    ElementBuffer& operator=(const ElementBuffer&) = delete;

    ElementBuffer& operator=(std::vector<uint32_t> indices);
    ElementBuffer& operator=(std::initializer_list<uint32_t> list);

    void create();
    void destroy();

    [[nodiscard]] size_t size() const noexcept;

    static void bind(const ElementBuffer& buffer);
    static void unbind();

private:
    std::vector<uint32_t> m_indices;
    uint32_t m_id;
};