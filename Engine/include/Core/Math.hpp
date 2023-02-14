#pragma once

#include "include/Core/Matrix.hpp"

namespace eg
{

	float radians(float degrees);

	Matrix4 ortho(
		float left,
		float right,
		float bottom,
		float top,
		float zNear,
		float zFar);

} // namespace eg