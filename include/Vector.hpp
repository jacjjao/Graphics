#pragma once


template <typename T>
class Vector2
{
public:
    explicit constexpr Vector2() noexcept = default;
    explicit constexpr Vector2(T x, T y) noexcept : x{x}, y{y}
    {
    }

    Vector2<T>& operator+=(const Vector2<T>& other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2<T> operator-() const noexcept
    {
        return Vector2<T>{-x, -y};
    }

    T x, y;
};

using Vector2f = Vector2<float>;

template <typename T>
class Vector3
{
public:
    T x, y, z;
};