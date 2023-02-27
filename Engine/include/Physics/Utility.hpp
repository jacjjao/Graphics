#pragma once

#include <cmath>
#include <include/Core/Vector.hpp>

namespace eg
{
	inline bool nearlyEqual(const float a, const float b)
	{
		return std::abs(a - b) < 0.0005f;
	}

	inline bool nearlyEqual(const Vector2f a, const Vector2f b)
	{
		return nearlyEqual(a.x, b.x) and nearlyEqual(a.y, b.y);
	}
} // namespace eg