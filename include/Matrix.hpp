#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

#if (DEBUG)
#include <iostream>
#endif

namespace detail
{

template <typename T, size_t Width>
class Row
{
public:
    explicit constexpr Row(T* ptr) noexcept : ptr_(ptr)
    {
    }

    constexpr T& operator[](const size_t index) noexcept
    {
#if (DEBUG)
        if (index >= Width)
        {
            std::cerr << "Index out of bounds! Index: " << index << " Size: " << Width << '\n';
        }
#endif
        return ptr_[index];
    }

    constexpr T& operator[](const size_t index) const noexcept
    {
#if (DEBUG)
        if (index >= Width)
        {
            std::cerr << "Index out of bounds! Index: " << index << " Size: " << Width << '\n';
        }
#endif
        return ptr_[index];
    }

    [[nodiscard]] constexpr size_t size() const noexcept
    {
        return Width;
    }

private:
    T* ptr_;
};

template <typename T, size_t Width>
class ConstRow
{
public:
    explicit constexpr ConstRow(const T* ptr) noexcept : ptr_(ptr)
    {
    }

    constexpr const T& operator[](const size_t index) const noexcept
    {
#if (DEBUG)
        if (index >= Width)
        {
            std::cerr << "Index out of bounds! Index: " << index << " Size: " << Width << '\n';
        }
#endif
        return ptr_[index];
    }

    [[nodiscard]] constexpr size_t size() const noexcept
    {
        return Width;
    }

private:
    const T* ptr_;
};

} // namespace detail

template <typename T, size_t Height, size_t Width>
class Matrix
{
public:
    static_assert(Width > 0, "Invalid width value");
    static_assert(Height > 0, "Invalid Height value");

    explicit Matrix() noexcept : items_(Width * Height)
    {
    }

    Matrix(std::initializer_list<T> list) noexcept : items_(list)
    {
        items_.resize(Width * Height);
    }

    ~Matrix() noexcept = default;

    Matrix(const Matrix<T, Height, Width>&)            = delete;
    Matrix& operator=(const Matrix<T, Height, Width>&) = delete;

    Matrix(Matrix<T, Height, Width>&&) noexcept            = default;
    Matrix& operator=(Matrix<T, Height, Width>&&) noexcept = default;

    detail::Row<T, Width> operator[](const size_t index) noexcept
    {
#if (DEBUG)
        if (index >= Height)
        {
            std::cerr << "Index out of bounds! Index: " << index << " Size: " << Height << '\n';
        }
#endif
        return detail::Row<T, Width>{&items_[index * Width]};
    }

    detail::ConstRow<T, Width> operator[](const size_t index) const noexcept
    {
#if (DEBUG)
        if (index >= Height)
        {
            std::cerr << "Index out of bounds! Index: " << index << " Size: " << Height << '\n';
        }
#endif
        return detail::ConstRow<T, Width>{&items_[index * Width]};
    }

    friend Matrix<T, Height, Width> operator+(const Matrix<T, Height, Width>& lhs, const Matrix<T, Height, Width>& rhs) noexcept
    {
        Matrix<T, Height, Width> result{};

        for (size_t i = 0; i < Height; i++)
        {
            for (size_t j = 0; j < Width; j++)
            {
                result[i][j] = lhs[i][j] + rhs[i][j];
            }
        }

        return result;
    }

    friend Matrix<T, Height, Width> operator-(const Matrix<T, Height, Width>& lhs, const Matrix<T, Height, Width>& rhs) noexcept
    {
        Matrix<T, Height, Width> result{};

        for (size_t i = 0; i < Height; i++)
        {
            for (size_t j = 0; j < Width; j++)
            {
                result[i][j] = lhs[i][j] - rhs[i][j];
            }
        }

        return result;
    }

    friend Matrix<T, Height, Width>& operator+=(Matrix<T, Height, Width>& lhs, const Matrix<T, Height, Width>& rhs) noexcept
    {
        for (size_t i = 0; i < Height; i++)
        {
            for (size_t j = 0; j < Width; j++)
            {
                lhs[i][j] += rhs[i][j];
            }
        }

        return lhs;
    }

    friend Matrix<T, Height, Width>& operator-=(Matrix<T, Height, Width>& lhs, const Matrix<T, Height, Width>& rhs) noexcept
    {
        for (size_t i = 0; i < Height; i++)
        {
            for (size_t j = 0; j < Width; j++)
            {
                lhs[i][j] -= rhs[i][j];
            }
        }

        return lhs;
    }

    template <size_t W>
    friend Matrix<T, Height, W> operator*(const Matrix<T, Height, Width>& lhs, const Matrix<T, Width, W>& rhs) noexcept
    {
        Matrix<T, Height, W> result{};

        for (size_t i = 0; i < Height; i++)
        {
            for (size_t j = 0; j < Width; j++)
            {
                for (size_t k = 0; k < W; k++)
                {
                    result[i][k] += lhs[i][j] * rhs[j][k];
                }
            }
        }

        return result;
    }

    [[nodiscard]] size_t width() const noexcept
    {
        return Width;
    }

    [[nodiscard]] size_t height() const noexcept
    {
        return Height;
    }

    [[nodiscard]] T* data() noexcept
    {
        return items_.data();
    }

    [[nodiscard]] const T* data() const noexcept
    {
        return items_.data();
    }

    void toIdentity() noexcept
    {
        static_assert(Width == Height, "Cannot turn to identity matrix");

        const auto zero = static_cast<T>(0);
        const auto one  = static_cast<T>(1);

        for (size_t i = 0; i < Height; i++)
        {
            for (size_t j = 0; j < Width; j++)
            {
                items_[i * Width + j] = (i == j) ? one : zero;
            }
        }
    }

    static Matrix<T, Height, Width> makeIdentity() noexcept
    {
        static_assert(Width == Height, "Invalid identity matrix size");

        Matrix<T, Height, Width> mat{};

        const auto one = static_cast<T>(1);
        for (size_t i = 0; i < Height; i++)
        {
            mat[i][i] = one;
        }

        return mat;
    }


private:
    std::vector<T> items_;
};


// using Matrix3 = Matrix<float, 3, 3>;
using Matrix4 = Matrix<float, 4, 4>;

static inline const Matrix4 identity_mat4 = Matrix4::makeIdentity();