#pragma once

#include "Core.hpp"
#include "Matrix.hpp"

namespace eg
{

	float EG_API radians(float degrees);

	Matrix4 EG_API ortho(
		float left,
		float right,
		float bottom,
		float top,
		float zNear,
		float zFar);

} // namespace eg