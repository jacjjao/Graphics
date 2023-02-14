#pragma once

#include "RigidBody.hpp"
#include <span>

namespace eg
{

	class Constraint
	{
	public:
		virtual void applyConstraint(RigidBody& body, const float dt) = 0;

		void applyConstraint(std::span<RigidBody*> bodies, const float dt)
		{
			for (auto& body : bodies) 
			{
				applyConstraint(*body, dt);
			}
		}
		
	};

	class CircleConstraint : Constraint
	{
	public:
		void applyConstraint(RigidBody& body, const float dt) override
		{
			const auto pos = body.centroid_pos;
			if (pos.x * pos.x + pos.y * pos.y <= 450.f * 450.f)
				return;

			const auto J = pos;
			const auto v = body.linear_velocity;
			const auto m = body.mass;
			const float lambda = -(J * v) / (pos * pos * m * dt);
			const auto fc = J * lambda;
			body.external_forces += fc;
		}
	};

} // namespace eg