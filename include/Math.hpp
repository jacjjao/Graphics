#pragma once

#include "Matrix.hpp"

float radians(float degrees) noexcept;

Matrix4 ortho(
	float left,
	float right, 
	float bottom, 
	float top, 
	float zNear, 
	float zFar) noexcept;