#pragma once

#include "include/Core/Vector.hpp"

namespace eg
{

	class RigidBody
	{
	public:
		Vector2f acceleration, linear_velocity, centroid_pos, external_forces;

		void updatePosition(const float dt)
		{
			acceleration = external_forces * m_mass_inv;
			linear_velocity = linear_velocity + acceleration * dt;
			centroid_pos = centroid_pos + linear_velocity * dt;

			external_forces = {};
		}

		void applyImpulse(const eg::Vector2f impulse)
		{
			linear_velocity += (impulse * m_mass_inv);
		}

		void setMass(const float mass)
		{
			m_mass = mass;
			m_mass_inv = 1.0f / mass;
		}

		float getMass() const
		{
			return m_mass;
		}

		float getInvMass() const
		{
			return m_mass_inv;
		}

	private:
		float m_mass = 1.0f, m_mass_inv = 1.0f;
	};

} // namespace eg