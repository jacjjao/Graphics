#pragma once

#include "include/Core/Vector.hpp"
#include "RigidBody.hpp"

#include <memory>
#include <vector>

namespace Engine
{

	class World
	{
	public:
		void setGravity(const Vector2f gravity) { m_gravity = gravity; }

		void update(const float dt)
		{
			for (auto& body : m_bodies) {
				body.applyForce(m_gravity * body.getMass()); 
				applyConstraint(body, dt);
				body.update(dt);
			}
		}

		void addBody(const RigidBody rd)
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
			auto pos = body.getCentroidPosition();
			const auto velocity = body.getVelocity();
			if (pos.y < -600 + 50) {
				pos.y = -600 + 50;
				body.setVelocity({});
				body.applyForce(-m_gravity);
			}
			body.setCentroidPosition(pos);
		}

		Vector2f m_gravity;
		std::vector<RigidBody> m_bodies;
	};

} // namespace Engine
