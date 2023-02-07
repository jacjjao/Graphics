#pragma once

#include "include/Core/Vector.hpp"

namespace Engine
{

	class RigidBody
	{
	public:
		void setCentroidPosition(const Vector2f position) { m_centroid_pos = position; };
		Vector2f getCentroidPosition() const { return m_centroid_pos; }

		void setMass(const float mass) { m_mass = mass; }
		float getMass() const { return m_mass; }

		void update(const float dt)
		{
			m_centroid_pos = m_centroid_pos + m_velocity * dt;
			m_velocity	   = m_velocity + m_acceleration * dt;
			m_acceleration = m_force / m_mass;

			m_force = {};
		}

		void applyForce(const Vector2f force)
		{
			m_force += force;
		}

	private:
		float m_mass = 1.0f;
		Vector2f m_acceleration, m_velocity, m_centroid_pos, m_force;
	};

} // namespace Engine