#pragma once


template <typename T>
class Vector2
{
public:
    explicit constexpr Vector2() noexcept = default;
    explicit constexpr Vector2(T x, T y) noexcept : x{x}, y{y}
    {
    }

    constexpr Vector2<T>& operator+=(const Vector2<T>& other) noexcept
    {
        x += other.x;
        y += other.y;

        return *this;
    }

    constexpr Vector2<T> operator-() const noexcept
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
    explicit constexpr Vector3() noexcept = default;
    explicit constexpr Vector3(T x, T y, T z) noexcept : x{x}, y{y}, z{z}
    {
    }

    explicit constexpr Vector3(const Vector2<T>& vector) noexcept : x{vector.x}, y{vector.y}, z{static_cast<T>(0)}
    {
    }

    constexpr Vector3<T>& operator=(const Vector2<T>& other) noexcept
    {
        x = other.x;
        y = other.y;
        z = T{};

        return *this;
    }

    constexpr Vector3<T>& operator+=(const Vector3<T>& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    constexpr Vector3<T> operator-() const noexcept
    {
        return Vector3<T>{-x, -y, -z};
    }

    T x, y, z;
};

using Vector3f = Vector3<float>;