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

    T x, y;
};

template <typename T>
struct Vector3
{
    T x, y, z;
};