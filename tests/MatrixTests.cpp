#include <gtest/gtest.h>

#include "../include/Matrix.hpp"

using Matrix2 = Matrix<int, 2, 2>;

TEST(Matrix, DefaultConstructor)
{
    Matrix2 mat{};

    auto* data = mat.data();

    EXPECT_NE(mat.data(), nullptr);
    for (int i = 0; i < 4; i++)
    {
        EXPECT_EQ(*(data + i), 0);
    }
}

TEST(Matrix, InitializerListConstructor)
{
    // clang-format off
	Matrix2 mat = {
		1, 2,
		3, 4
	};
    // clang-format on

    auto* data = mat.data();
    for (int i = 0; i < 4; i++)
    {
        EXPECT_EQ(*(data + i), i + 1);
    }
}

TEST(Matrix, MoveConstructor)
{
    Matrix2 mat{};

    auto* data = mat.data();

    Matrix2 mat2{std::move(mat)};

    EXPECT_NE(mat.data(), data);
    EXPECT_EQ(mat2.data(), data);
}

TEST(Matrix, MoveAssignment)
{
    Matrix2 mat{};
    auto*   data = mat.data();

    Matrix2 mat2{};
    auto*   data2 = mat2.data();

    mat2 = std::move(mat);

    EXPECT_EQ(mat2.data(), data);
    EXPECT_NE(mat2.data(), data2);
}

TEST(Matrix, OperatorSubscript)
{
    Matrix2 mat{};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            mat[i][j] = i + j;
        }
    }

    auto* data = mat.data();

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            EXPECT_EQ(*(data + 2 * i + j), i + j);
        }
    }
}

TEST(Matrix, ConstOperatorSubscript)
{
    Matrix2 mat{};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            mat[i][j] = i + j;
        }
    }

    const auto& mat2 = mat;

    auto* data = mat.data();
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            EXPECT_EQ(*(data + 2 * i + j), mat2[i][j]);
        }
    }
}

TEST(Matrix, PlusOperator)
{
    Matrix2 mat1{};
    Matrix2 mat2{};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            mat1[i][j] = i + j;
            mat2[i][j] = 1;
        }
    }

    const auto mat3 = mat1 + mat2;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            EXPECT_EQ(mat3[i][j], i + j + 1);
        }
    }
}

TEST(Matrix, MinusOperator)
{
    Matrix2 mat1{};
    Matrix2 mat2{};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            mat1[i][j] = i + j;
            mat2[i][j] = 1;
        }
    }

    const auto mat3 = mat1 - mat2;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            EXPECT_EQ(mat3[i][j], i + j - 1);
        }
    }
}

TEST(Matrix, PlusEqualOperator)
{
    Matrix2 mat1{};
    Matrix2 mat2{};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            mat1[i][j] = i + j;
            mat2[i][j] = 1;
        }
    }

    mat1 += mat2;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            EXPECT_EQ(mat1[i][j], i + j + 1);
        }
    }
}

TEST(Matrix, MinusEqualOperator)
{
    Matrix2 mat1{};
    Matrix2 mat2{};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            mat1[i][j] = i + j;
            mat2[i][j] = 1;
        }
    }

    mat1 -= mat2;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            EXPECT_EQ(mat1[i][j], i + j - 1);
        }
    }
}

TEST(Matrix, MultiplyOperator)
{
    // clang-format off
    Matrix2 mat1 = {
		1, 2,
		3, 4
	};
    Matrix2 mat2 = {
		5, 6,
		7, 8
	};
    // clang-format on

    const auto mat3 = mat1 * mat2;

    EXPECT_EQ(mat3[0][0], 19);
    EXPECT_EQ(mat3[0][1], 22);
    EXPECT_EQ(mat3[1][0], 43);
    EXPECT_EQ(mat3[1][1], 50);
}

TEST(Matrix, sizeFunction)
{
    const size_t width  = 10;
    const size_t height = 3;

    Matrix<int, height, width> mat{};

    EXPECT_EQ(mat.width(), width);
    EXPECT_EQ(mat.height(), height);
}

TEST(Matrix, ToIdentity)
{
    // clang-format off
	Matrix<int, 4, 4> mat = {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 1, 2, 3,
		4, 5, 6, 7
	};
    // clang-format on

    mat.toIdentity();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            EXPECT_EQ(mat[i][j], (i == j) ? 1 : 0);
        }
    }
}

TEST(Matrix, MakeIdentity)
{
    Matrix<int, 4, 4> mat = Matrix<int, 4, 4>::makeIdentity();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            EXPECT_EQ(mat[i][j], (i == j) ? 1 : 0);
        }
    }
}