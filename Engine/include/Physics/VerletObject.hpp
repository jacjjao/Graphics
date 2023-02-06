#pragma once

#include "include/Core/Vector.hpp"

namespace Engine
{

	class VerletObject
	{
	public:	
		VerletObject() = default;

		explicit VerletObject(const Vector2f pos, const float radius) : 
		m_radius{radius},
		old_pos{pos},
		current_pos{pos},
		acceleration{}
		{
		}

		void update(const float dt)
		{
			const Vector2f displacement = current_pos - old_pos;

			old_pos		= current_pos;
			current_pos = current_pos + displacement + acceleration * (dt * dt) * 0.5f;

			acceleration = {};
		}

		void accelerate(const Vector2f a)
		{
			acceleration += a;
		}

		void setAcceleration(const Vector2f a)
		{
			acceleration = a;
		}

		[[nodiscard]]
		Vector2f getPosition() const
		{
			return current_pos;
		}

		void setPosition(const Vector2f pos)
		{
			current_pos = pos;
		}

		void setVelocity(const Vector2f velocity, const float dt)
		{
			old_pos = current_pos - velocity * dt;
		}

		[[nodiscard]]
		float getRadius() const
		{
			return m_radius;
		}

	private:
		Vector2f old_pos, current_pos, acceleration;
		float m_radius;
	};

} // namespace Engine