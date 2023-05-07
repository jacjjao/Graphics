#pragma once

#include <cstdint>
#include <cassert>
#include <initializer_list>
#include <array>

namespace eg
{

    template <typename T, size_t Height, size_t Width>
    class Matrix
    {
    public:
        static_assert(Width > 0, "Invalid width value");
        static_assert(Height > 0, "Invalid Height value");

        /*
        Matrix(std::initializer_list<T> list) : items_(list)
        {
            assert(list.size() == Width * Height);
            items_.resize(Width * Height);
        }
        */

        Matrix(const Matrix&) = delete;
        Matrix& operator=(Matrix&) = delete;

        std::array<T, Width>& operator[](const size_t index)
        {
            return items_[index];
        }

        const std::array<T, Width>& operator[](const size_t index) const
        {
            return items_[index];
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

        friend Matrix<T, Height, Width>& operator-=(Matrix<T, Height, Width>& lhs, const Matrix<T, Height, Width>& rhs)
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
        friend Matrix<T, Height, W> operator*(const Matrix<T, Height, Width>& lhs, const Matrix<T, Width, W>& rhs)
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

        [[nodiscard]] 
        size_t width() const
        {
            return Width;
        }

        [[nodiscard]] 
        size_t height() const
        {
            return Height;
        }

        [[nodiscard]] 
        T* data()
        {
            return items_.begin()->data();
        }

        [[nodiscard]] 
        const T* data() const
        {
            return items_.begin()->data();
        }
        
        [[nodiscard]]
        static Matrix<T, Height, Width> makeIdentity()
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
        std::array<std::array<T, Width>, Height> items_;
    };

    using Matrix3 = Matrix<float, 3, 3>;
    using Matrix4 = Matrix<float, 4, 4>;

    namespace Constants
    {

        static inline const Matrix4 identity_mat4 = Matrix4::makeIdentity();

    } // namespace Constants

} // namespace eg