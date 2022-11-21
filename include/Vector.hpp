#pragma once


template <typename T>
struct Vector2
{
    Vector2<T>& operator+=(const Vector2<T>& other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2<T> operator-() const noexcept
    {
        return {-x, -y};
    }

    T x, y;
};

using Vector2f = Vector2<float>;

template <typename T>
struct Vector3
{
    T x, y, z;
};