#pragma once

#include "RigidBody.hpp"

namespace Engine
{

	class Constraint
	{
	public:
		virtual void applyConstraint(RigidBody& body) = 0;
	};

	class CircleConstraint : Constraint
	{
	public:
		void applyConstraint(RigidBody& body) override
		{

		}
	};

} // namespace Engine