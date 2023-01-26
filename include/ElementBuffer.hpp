#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

class ElementBuffer
{
public:
    explicit ElementBuffer(size_t size) noexcept;
    ~ElementBuffer() noexcept;

    ElementBuffer(const ElementBuffer&)            = delete;
    ElementBuffer& operator=(const ElementBuffer&) = delete;

    ElementBuffer(ElementBuffer&& other) noexcept;
    ElementBuffer& operator=(ElementBuffer&& other) noexcept;

    ElementBuffer& operator=(std::vector<uint32_t> indices) noexcept;
    ElementBuffer& operator=(std::initializer_list<uint32_t> list) noexcept;

    void update() noexcept;

    void destroy() noexcept;

    [[nodiscard]] size_t size() const noexcept;

    [[nodiscard]] bool isCreated() const noexcept;

    static void bind(ElementBuffer* ebo) noexcept;
    static void unbind() noexcept;

private:
    void create() noexcept;

    static ElementBuffer* ebo_in_bind;

    std::vector<uint32_t> m_indices;
    uint32_t              m_id;
};