#pragma once

#include "RigidBody.hpp"
#include <span>

namespace eg
{

	class Constraint
	{
	public:
		virtual void applyConstraint(RigidBody& body, const float dt) = 0;
	};

	class CircleConstraint : public Constraint
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

	class FloorConstraint : public Constraint
	{
	public:
		void applyConstraint(RigidBody& body, const float dt) override
		{
			const auto pos = body.centroid_pos;
			if (pos.y - obj_half_height >= floor_h)
				return;

			constexpr auto bau_term = 0.4f; // baumgarte term
			const auto bias = -(pos.y - obj_half_height - floor_h) * bau_term / dt;
			const eg::Vector2f fc{ 0.0f, (bias - body.linear_velocity.y) * body.mass / dt };
			body.external_forces += fc;
		}

		float floor_h = 0.0f;
		float obj_half_height = 0.0f;
	};

} // namespace eg