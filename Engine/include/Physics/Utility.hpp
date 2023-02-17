#pragma once

#include <cmath>

namespace eg
{
	inline bool nearlyEqual(const float a, const float b)
	{
		return std::abs(a - b) < 0.0005f;
	}
} // namespace eg