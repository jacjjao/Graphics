#include <gtest/gtest.h>

#include "../include/Vector.hpp"
#include <cmath>

TEST(Vector, DefaultConstructor)
{
    {
        Vector2f vec{};

        EXPECT_EQ(vec.x, 0.0F);
        EXPECT_EQ(vec.y, 0.0F);
    }
    {
        constexpr Vector2f vec{};

        EXPECT_EQ(vec.x, 0.0F);
        EXPECT_EQ(vec.y, 0.0F);
    }
    {
        Vector3f vec{};

        EXPECT_EQ(vec.x, 0.0F);
        EXPECT_EQ(vec.y, 0.0F);
        EXPECT_EQ(vec.z, 0.0F);
    }
    {
        constexpr Vector3f vec{};

        EXPECT_EQ(vec.x, 0.0F);
        EXPECT_EQ(vec.y, 0.0F);
        EXPECT_EQ(vec.z, 0.0F);
    }
}

TEST(Vector, ConstructorWithParameters)
{
    {
        {
            Vector2f vec{1.0F, 2.0F};

            EXPECT_EQ(vec.x, 1.0F);
            EXPECT_EQ(vec.y, 2.0F);
        }
        {
            constexpr Vector2f vec{1.0F, 2.0F};

            EXPECT_EQ(vec.x, 1.0F);
            EXPECT_EQ(vec.y, 2.0F);
        }
        {
            Vector3f vec{1.0F, 2.0F, 3.0F};

            EXPECT_EQ(vec.x, 1.0F);
            EXPECT_EQ(vec.y, 2.0F);
            EXPECT_EQ(vec.z, 3.0F);
        }
        {
            constexpr Vector3f vec{1.0F, 2.0F, 3.0F};

            EXPECT_EQ(vec.x, 1.0F);
            EXPECT_EQ(vec.y, 2.0F);
            EXPECT_EQ(vec.z, 3.0F);
        }
    }
}

TEST(Vector, PlusEqualOperator)
{
    {
        Vector2f       vec1{1.0F, 2.0F};
        const Vector2f vec2{2.0F, 3.0F};

        vec1 += vec2;

        EXPECT_EQ(vec1.x, 3.0F);
        EXPECT_EQ(vec1.y, 5.0F);
    }
    {
        Vector3f       vec1{1.0F, 2.0F, 3.0F};
        const Vector3f vec2{2.0F, 3.0F, 4.0F};

        vec1 += vec2;

        EXPECT_EQ(vec1.x, 3.0F);
        EXPECT_EQ(vec1.y, 5.0F);
        EXPECT_EQ(vec1.z, 7.0F);
    }
}

TEST(Vector, PrefixMinusOperator)
{
    {
        const Vector2f vec1{1.0F, 2.0F};

        const Vector2f vec2 = -vec1;

        EXPECT_EQ(vec2.x, -1.0F);
        EXPECT_EQ(vec2.y, -2.0F);
    }
    {
        const Vector3f vec1{1.0F, 2.0F, 3.0F};

        const Vector3f vec2 = -vec1;

        EXPECT_EQ(vec2.x, -1.0F);
        EXPECT_EQ(vec2.y, -2.0F);
        EXPECT_EQ(vec2.z, -3.0F);
    }

    {
        constexpr Vector2f vec1{1.0F, 2.0F};

        constexpr Vector2f vec2 = -vec1;

        EXPECT_EQ(vec2.x, -1.0F);
        EXPECT_EQ(vec2.y, -2.0F);
    }
    {
        constexpr Vector3f vec1{1.0F, 2.0F, 3.0F};

        constexpr Vector3f vec2 = -vec1;

        EXPECT_EQ(vec2.x, -1.0F);
        EXPECT_EQ(vec2.y, -2.0F);
        EXPECT_EQ(vec2.z, -3.0F);
    }
}

TEST(Vector, DotProductOperator)
{
    {
        const Vector2f vec1{1.0F, 2.0F};
        const Vector2f vec2{2.0F, 3.0F};

        const auto result = vec1 * vec2;

        EXPECT_EQ(result.x, 2.0F);
        EXPECT_EQ(result.y, 6.0F);
    }
    {
        const Vector3f vec1{1.0F, 2.0F, 3.0F};
        const Vector3f vec2{2.0F, 3.0F, 4.0F};

        const auto result = vec1 * vec2;

        EXPECT_EQ(result.x, 2.0F);
        EXPECT_EQ(result.y, 6.0F);
        EXPECT_EQ(result.z, 12.0F);
    }

    {
        constexpr Vector2f vec1{1.0F, 2.0F};
        constexpr Vector2f vec2{2.0F, 3.0F};

        constexpr Vector2f result = vec1 * vec2;

        EXPECT_EQ(result.x, 2.0F);
        EXPECT_EQ(result.y, 6.0F);
    }
    {
        constexpr Vector3f vec1{1.0F, 2.0F, 3.0F};
        constexpr Vector3f vec2{2.0F, 3.0F, 4.0F};

        constexpr Vector3f result = vec1 * vec2;

        EXPECT_EQ(result.x, 2.0F);
        EXPECT_EQ(result.y, 6.0F);
        EXPECT_EQ(result.z, 12.0F);
    }
}

TEST(Vector, Length)
{
    {
        const Vector2f vec{3.0F, 4.0F};

        EXPECT_EQ(vec.length(), 5.0F);
    }
    {
        const Vector3f vec{1.0F, 2.0F, 3.0F};

        EXPECT_EQ(vec.length(), std::sqrt(14.0F));
    }
}

TEST(Vector, Normalize)
{
    {
        const Vector2f vec{3.0F, 4.0F};

        const auto vec2 = vec.normalize();

        EXPECT_EQ(vec2.x, 0.6F);
        EXPECT_EQ(vec2.y, 0.8F);
    }
    {
        const Vector3f vec{1.0F, 2.0F, 3.0F};

        const auto vec2 = vec.normalize();

        EXPECT_EQ(vec2.x, 1.0F / std::sqrt(14.0F));
        EXPECT_EQ(vec2.y, 2.0F / std::sqrt(14.0F));
        EXPECT_EQ(vec2.z, 3.0F / std::sqrt(14.0F));
    }
}

TEST(Vector, CrossProduct)
{
    {
        const Vector3f vec1{1.0F, 0.0F, 0.0F};
        const Vector3f vec2{0.0F, 1.0F, 0.0F};

        const auto result = crossProduct(vec1, vec2);

        EXPECT_EQ(result.x, 0.0F);
        EXPECT_EQ(result.y, 0.0F);
        EXPECT_EQ(result.z, 1.0F);
    }
    {
        const Vector3f vec1{1.0F, 2.0F, 3.0F};
        const Vector3f vec2{4.0F, 5.0F, 6.0F};

        const auto result = crossProduct(vec1, vec2);

        EXPECT_EQ(result.x, -3.0F);
        EXPECT_EQ(result.y, 6.0F);
        EXPECT_EQ(result.z, -3.0F);
    }
    {
        constexpr Vector3f vec1{1.0F, 0.0F, 0.0F};
        constexpr Vector3f vec2{0.0F, 1.0F, 0.0F};

        constexpr Vector3f result = crossProduct(vec1, vec2);

        EXPECT_EQ(result.x, 0.0F);
        EXPECT_EQ(result.y, 0.0F);
        EXPECT_EQ(result.z, 1.0F);
    }
}