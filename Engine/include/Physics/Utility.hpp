#pragma once

#include <cmath>

namespace eg
{
	inline bool nearlyEqual(const float a, const float b)
	{
		return std::abs(a - b) < 0.0005f;
	}

	inline bool certainLess(const float a, const float b)
	{
		return not nearlyEqual(a, b) and a < b;
	}

	inline bool certainGreater(const float a, const float b)
	{
		return not nearlyEqual(a, b) and a > b;
	}
} // namespace eg