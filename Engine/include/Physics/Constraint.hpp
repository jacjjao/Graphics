#pragma once

#include "RigidBody.hpp"
#include <span>

namespace Engine
{

	class Constraint
	{
	public:
		virtual void applyConstraint(RigidBody& body, const float dt) = 0;

		virtual void applyConstraint(std::span<RigidBody*> bodies, const float dt)
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
			const auto pos = body.getCentroidPosition();
			if (pos.x * pos.x + pos.y * pos.y <= 450.f * 450.f)
				return;

			const auto J = pos;
			//const auto vprev = body.getVelocity();
			//const auto vdelta = body.getAcceleration() * dt;
			//const auto v = vprev + vdelta
			const auto v = body.getVelocity();
			const auto m = body.getMass();
			const float lambda = -(J * v) / (pos * pos * m * dt);
			const auto fc = J * lambda;
			body.applyForce(fc);
		}
	};

} // namespace Engine