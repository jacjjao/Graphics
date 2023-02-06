#pragma once 

#include "include/Core/Vector.hpp"
#include "include/Physics/VerletObject.hpp"
#include <vector>

namespace Engine
{

	class Solver
	{
	public:
		explicit Solver(const unsigned update_rate, const float constraint_radius) : 
			m_update_rate{update_rate},
			m_constraint_radius{constraint_radius}
		{
			m_step_dt = 1.0f / (static_cast<float>(m_update_rate) * static_cast<float>(m_step));
		}

		void update()
		{
			const auto dtt = getStepDt();
			for (int i = 0; i < m_step; i++) {
				applyGravity();
				detectCollision();
				applyConstraint();
				updateObjects(dtt);
			}
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

		void setStep(const unsigned step)
		{
			m_step = step;
		}

		[[nodiscard]]
		float getStepDt() const
		{
			return m_step_dt;
		}

	private:
		static constexpr Vector2f gravity = { 0.0f, -2000.0f };

		std::vector<VerletObject> objs;

		unsigned m_step = 8;
		unsigned m_update_rate = 0;
		float m_step_dt;
		float m_constraint_radius;
		Engine::Clock m_clock;

		void applyGravity()
		{
			for (auto& obj : objs)
				obj.accelerate(gravity);
		}

		void detectCollision()
		{
			// brute force O(n^2)
			for (size_t i = 0, n = objs.size(); i < n; i++) {
				for (size_t j = i + 1; j < n; j++) {
					const auto collision_axis = objs[j].getPosition() - objs[i].getPosition();
					const auto obj_dist = collision_axis.length();
					const auto radius_sum = objs[i].getRadius() + objs[j].getRadius();
					if (obj_dist < radius_sum) {
						const auto unit_axis = collision_axis.normalize();
						const auto displace_len = (radius_sum - obj_dist) / 2.0f;
						objs[i].setPosition(objs[i].getPosition() - unit_axis * displace_len);
						objs[j].setPosition(objs[j].getPosition() + unit_axis * displace_len);
					}
				}
			}
		}

		void applyConstraint()
		{
			for (auto& obj : objs) {
				const auto pos = obj.getPosition();
				const auto dist = pos.length();
				if (dist >= (m_constraint_radius - obj.getRadius())) {
					const Vector2f unit_v = pos.normalize();
					obj.setPosition(unit_v * (m_constraint_radius - obj.getRadius()));
				}
			}
		}

		void updateObjects(const float dt)
		{
			for (auto& obj : objs)
				obj.update(dt);
		}
	};

} // Engine