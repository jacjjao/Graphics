#pragma once 

#include "include/Core/Vector.hpp"
#include "include/Physics/VerletObject.hpp"
#include <vector>

namespace Engine
{

	class Solver
	{
	public:
		void update(const float dt)
		{
			applyGravity();
			applyConstraint();
			updateObjects(dt);
		}

		void addObject(VerletObject obj)
		{
			objs.push_back(std::move(obj));
		}

		[[nodiscard]]
		const std::vector<VerletObject>& getObjects() const
		{
			return objs;
		}

	private:
		void applyGravity()
		{
			for (auto& obj : objs)
				obj.accelerate(gravity);
		}

		void applyConstraint()
		{
			for (auto& obj : objs) {
				const auto pos = obj.getPosition();
				const auto dist = pos.length();
				if (dist >= (500 - 50)) {
					const Vector2f unit_v = pos / dist;
					obj.setPosition(unit_v * (500 - 50));
				}
			}
		}

		void updateObjects(const float dt)
		{
			for (auto& obj : objs)
				obj.update(dt);
		}

		static constexpr Vector2f gravity = { 0.0f, -2000.0f };
		
		std::vector<VerletObject> objs;
	};

} // Engine