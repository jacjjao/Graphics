#pragma once

#include <array>
#include <concepts>
#include <cstdint>
#include <utility>
#include <vector>

namespace detail
{

template <typename T>
concept ValueType = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <ValueType T, size_t Width>
class MatrixRow
{
public:
    explicit MatrixRow(T* ptr) : ptr_(ptr)
    {
    }

    T& operator[](const size_t index) noexcept
    {
        return ptr_[index];
    }

    const T& operator[](const size_t index) const noexcept
    {
        return ptr_[index];
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return Width;
    }

private:
    T* ptr_;
};

template <ValueType T, size_t Width>
class ConstMatrixRow
{
public:
    explicit ConstMatrixRow(const T* ptr) : ptr_(ptr)
    {
    }

    const T& operator[](const size_t index) const noexcept
    {
        return ptr_[index];
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return Width;
    }

private:
    const T* ptr_;
};

} // namespace detail

template <detail::ValueType T, size_t Height, size_t Width>
class Matrix
{
public:
    static_assert(Width > 0, "Invalid width value");
    static_assert(Height > 0, "Invalid Height value");

    explicit Matrix(const T init = T{}) : items_(Width * Height, init)
    {
    }

    explicit Matrix(std::array<T, Width * Height> arr) : items_(arr.begin(), arr.end())
    {
    }

    detail::MatrixRow<T, Width> operator[](const size_t index) noexcept
    {
        return detail::MatrixRow<T, Width>{&items_[index * Width]};
    }

    detail::ConstMatrixRow<T, Width> operator[](const size_t index) const noexcept
    {
        return detail::ConstMatrixRow<T, Width>{&items_[index * Width]};
    }

    friend Matrix<T, Height, Width> operator+(const Matrix<T, Height, Width>& lhs, const Matrix<T, Height, Width>& rhs)
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

    friend Matrix<T, Height, Width> operator-(const Matrix<T, Height, Width>& lhs, const Matrix<T, Height, Width>& rhs)
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

    friend Matrix<T, Height, Width>& operator+=(Matrix<T, Height, Width>& lhs, const Matrix<T, Height, Width>& rhs)
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

    typename std::vector<T>::iterator begin() const noexcept
    {
        return items_.begin();
    }

    typename std::vector<T>::iterator end() const noexcept
    {
        return items_.end();
    }

    typename std::vector<T>::iterator cbegin() const noexcept
    {
        return items_.cbegin();
    }

    typename std::vector<T>::iterator cend() const noexcept
    {
        return items_.cend();
    }

    [[nodiscard]] size_t width() const noexcept
    {
        return Width;
    }

    [[nodiscard]] size_t height() const noexcept
    {
        return Height;
    }

    static Matrix<T, Height, Width> identity() noexcept
    {
        static_assert(Width == Height, "Invalid identity matrix size");

        Matrix<T, Height, Width> mat{};

        T one = static_cast<T>(1);
        for (size_t i = 0; i < Height; i++)
        {
            mat[i][i] = one;
        }

        return mat;
    }

private:
    std::vector<T> items_;
};