#pragma once

#include "Matrix.hpp"

namespace Engine
{

	float radians(float degrees);

	Matrix4 ortho(
		float left,
		float right,
		float bottom,
		float top,
		float zNear,
		float zFar);

} // namespace Engine