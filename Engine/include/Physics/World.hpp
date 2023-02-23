#pragma once

#include "include/Core/Vector.hpp"
#include "RigidBody.hpp"

#include <memory>
#include <vector>

namespace eg
{
	namespace physics
	{
		class World
		{
		public:
			void setGravity(const Vector2f gravity) { m_gravity = gravity; }

			void update(const float dt)
			{
			}

			void addBody(const RigidBody& rd)
			{
				m_bodies.push_back(rd);
			}

			const std::vector<RigidBody>& getBodies() const
			{
				return m_bodies;
			}

		private:
			void applyConstraint(RigidBody& body, const float dt)
			{
			}

			Vector2f m_gravity;
			std::vector<RigidBody> m_bodies;
		};
	} // physics

} // namespace eg
