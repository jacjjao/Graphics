#pragma once

#include "include/Core/Vector.hpp"

namespace eg
{

	class RigidBody
	{
	public:
		void updateVelocity(const float dt)
		{
			acceleration = external_forces / mass;
			linear_velocity = linear_velocity + acceleration * dt;

			external_forces = {};
		}

		void updatePosition(const float dt)
		{
			acceleration = external_forces / mass;
			linear_velocity = linear_velocity + acceleration * dt;
			centroid_pos = centroid_pos + linear_velocity * dt + acceleration * (dt * dt);

			external_forces = {};
		}

		float mass = 1.0f;
		Vector2f acceleration, linear_velocity, centroid_pos, external_forces;
	};

} // namespace eg