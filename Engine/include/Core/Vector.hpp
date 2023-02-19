#pragma once

#include <cmath>

namespace eg
{

    template <typename T>
    class Vector2
    {
    public:
        constexpr Vector2(T val = static_cast<T>(0)) : x{ val }, y{ val }
        {
        }

        constexpr Vector2(T x, T y) : x{ x }, y{ y }
        {
        }

        constexpr bool operator==(const Vector2<T> other) const
        {
            return (x == other.x) and (y == other.y);
        }

        constexpr Vector2<T>& operator+=(const Vector2<T> other)
        {
            x += other.x;
            y += other.y;

            return *this;
        }

        constexpr Vector2<T> operator+(const Vector2<T> other) const
        {
            return { this->x + other.x, this->y + other.y };
        }

        constexpr Vector2<T> operator-() const
        {
            return { -x, -y };
        }

        constexpr Vector2<T> operator-(const Vector2<T> other) const
        {
            return { this->x - other.x, this->y - other.y };
        }

        constexpr T operator*(const Vector2<T> other) const
        {
            return x * other.x + y * other.y;
        }

        constexpr Vector2<T> operator*(const T scalar) const
        {
            return { x * scalar, y * scalar };
        }

        constexpr Vector2<T> operator/(const T factor) const
        {
            return { x / factor, y / factor };
        }

        [[nodiscard]] 
        T length() const
        {
            return std::sqrt(x * x + y * y);
        }

        [[nodiscard]]
        T lengthSquared() const
        {
            return x* x + y * y;
        }

        [[nodiscard]] 
        Vector2<T> normalize() const
        {
            const auto len = length();

            return Vector2<T>{x / len, y / len};
        }

        [[nodiscard]]
        T cross(const Vector2<T> other) const 
        {
            return x * other.y - y * other.x;
        }

        T x, y;
    };

    using Vector2f = Vector2<float>;

    template <typename T>
    class Vector3
    {
    public:
        constexpr Vector3(T val = static_cast<T>(0)) : x{ val }, y{ val }, z{ val }
        {
        }

        constexpr Vector3(T x, T y, T z) : x{ x }, y{ y }, z{ z }
        {
        }

        explicit constexpr Vector3(const Vector2<T>& vector) : x{ vector.x }, y{ vector.y }, z{ static_cast<T>(0) }
        {
        }

        constexpr Vector3<T>& operator=(const Vector2<T>& other)
        {
            x = other.x;
            y = other.y;
            z = static_cast<T>(0);

            return *this;
        }

        constexpr Vector3<T>& operator+=(const Vector3<T>& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;

            return *this;
        }

        constexpr Vector3<T> operator+(const Vector3<T>& other) const
        {
            Vector3<T> result{};

            result.x = this->x + other.x;
            result.y = this->y + other.y;
            result.z = this->z + other.z;

            return result;
        }

        constexpr Vector3<T> operator-(const Vector3<T>& other) const
        {
            Vector3<T> result{};

            result.x = this->x - other.x;
            result.y = this->y - other.y;
            result.z = this->z - other.z;

            return result;
        }

        constexpr Vector3<T> operator-() const
        {
            return Vector3<T>{-x, -y, -z};
        }

        [[nodiscard]] 
        T length() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        [[nodiscard]] 
        Vector3<T> normalize() const
        {
            const auto len = length();

            return Vector3<T>{x / len, y / len, z / len};
        }

        static constexpr Vector3<T> cross(const Vector3<T>& lhs, const Vector3<T>& rhs)
        {
            Vector3<T> result{};

            result.x = lhs.y * rhs.z - lhs.z * rhs.y;
            result.y = lhs.z * rhs.x - lhs.x * rhs.z;
            result.z = lhs.x * rhs.y - lhs.y * rhs.x;

            return result;
        }

        static constexpr Vector3<T> dot(const Vector3<T>& lhs, const Vector3<T>& rhs)
        {
            return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
        }

        T x, y, z;
    };

    using Vector3f = Vector3<float>;

} // namespace eg