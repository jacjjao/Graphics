#pragma once

#include "Core.hpp"
#include <cstdint>
#include <cassert>
#include <initializer_list>
#include <array>

namespace eg
{

    template <typename T, size_t Height, size_t Width>
    class EG_API Matrix
    {
    public:
        static_assert(Width > 0, "Invalid width value");
        static_assert(Height > 0, "Invalid Height value");

        // Matrix(const Matrix&) = delete;
        // Matrix& operator=(Matrix&) = delete;
        
        std::array<T, Width>& operator[](const size_t index)
        {
            return items_[index];
        }
                
        const std::array<T, Width>& operator[](const size_t index) const
        {
            return items_[index];
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

} // namespace eg