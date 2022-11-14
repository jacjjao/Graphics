#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

class ElementBuffer
{
public:
    explicit ElementBuffer(size_t size);
    ~ElementBuffer();

    ElementBuffer(ElementBuffer&) = delete;
    ElementBuffer& operator=(ElementBuffer&) = delete;

    ElementBuffer& operator=(std::initializer_list<uint32_t> list);

    void bind() const;
    void create();
    void destroy();

    [[nodiscard]] size_t size() const noexcept;

    static void unbind();

private:
    std::vector<uint32_t> m_indices;
    uint32_t m_id;
};