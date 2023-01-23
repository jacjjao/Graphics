#pragma once

#include <cmath>

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

    constexpr Vector2<T> operator+(const Vector2<T>& other) const noexcept
    {
        return Vector2<T>{this->x + other.x, this->y + other.y};
    }

    constexpr Vector2<T> operator-() const noexcept
    {
        return Vector2<T>{-x, -y};
    }

    constexpr Vector2<T> operator*(const Vector2<T>& other) const noexcept
    {
        return Vector2<T>{x * other.x, y * other.y};
    }

    constexpr Vector2<T> operator/(const T& factor) const noexcept
    {
        return Vector2<T>{x / factor, y / factor};
    }

    [[nodiscard]] T length() const noexcept
    {
        return std::sqrt(x * x + y * y);
    }

    [[nodiscard]] Vector2<T> normalize() const noexcept
    {
        const auto len = length();

        return Vector2<T>{x / len, y / len};
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

    Vector3<T>& operator+=(const Vector3<T>& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    Vector3<T> operator+(const Vector3<T>& other) const noexcept
    {
        Vector3<T> result{};

        result.x = this->x + other.x;
        result.y = this->y + other.y;
        result.z = this->z + other.z;

        return result;
    }

    Vector3<T> operator-(const Vector3<T>& other) const noexcept
    {
        Vector3<T> result{};

        result.x = this->x - other.x;
        result.y = this->y - other.y;
        result.z = this->z - other.z;

        return result;
    }

    constexpr Vector3<T> operator-() const noexcept
    {
        return Vector3<T>{-x, -y, -z};
    }

    constexpr Vector3<T> operator*(const Vector3<T>& other) const noexcept
    {
        return Vector3<T>{x * other.x, y * other.y, z * other.z};
    }

    [[nodiscard]] T length() const noexcept
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    [[nodiscard]] Vector3<T> normalize() const noexcept
    {
        const auto len = length();

        return Vector3<T>{x / len, y / len, z / len};
    }

    T x, y, z;
};

using Vector3f = Vector3<float>;

template <typename T>
constexpr Vector3<T> crossProduct(const Vector3<T>& a, const Vector3<T>& b) noexcept
{
    Vector3<T> result{};

    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;

    return result;
}