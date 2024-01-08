#pragma once

#include "include/Core/Vector.hpp"
#include <numbers>

namespace eg
{

	namespace physics
	{
		template<typename T>
		std::pair<T, T> eulerODESolver(const T x, const T v, const T a, const float dt)
		{
            const auto x_new = x + v * dt;
            const auto v_new = v + a * dt;
			return { x_new, v_new };
		}

		enum class RigidBodyType
		{
			Box, Circle
		};

		class RigidBody
		{
		public:
			Vector2f linear_velocity{}, position{}, external_forces{};
			float angular_velocity = 0.0f, angular_acceleration, rotate_radians = 0.0f;
			float width = 0.0f, height = 0.0f, radius = 0.0f;
			RigidBodyType type = RigidBodyType::Box;
			bool is_static = false;

			void update(const float dt)
			{
				if (is_static)
				{
					return;
				}
				const auto acceleration = external_forces * m_inverse_mass;
				const auto [x, v] = eulerODESolver(position, linear_velocity, acceleration, dt);
				position = x;
				linear_velocity = v;

				const auto [ax, av] = eulerODESolver(rotate_radians, angular_velocity, angular_acceleration, dt);
				rotate_radians = ax;
				angular_velocity = av;

				external_forces = {};
			}

			void applyImpulse(const eg::Vector2f impulse)
			{
				linear_velocity += impulse * m_inverse_mass;
			}

			void setMass(const float mass)
			{
				m_mass = mass;
				m_inverse_mass = 1.0f / mass;
			}

			[[nodiscard]]
			float getMass() const
			{
				return m_mass;
			}

			[[nodiscard]]
			float getInverseMass() const
			{
				return m_inverse_mass;
			}

			void calcInertia()
			{
				switch (type)
				{
				case RigidBodyType::Box:
				{
					constexpr float c = 1.0f / 12.0f;
					m_inertia = c * m_mass * (width * width + height * height);
					break;
				}
				case RigidBodyType::Circle:
					m_inertia = 0.5f * m_mass * (radius * radius);
					break;
				}
				m_inverse_inertia = 1.0f / m_inertia;
			}

			void applyInertiaTensor(const eg::Vector2f r, const eg::Vector2f jr)
			{
				angular_velocity += r.cross(jr) * m_inverse_inertia;
			}

			[[nodiscard]]
			float getInertia() const
			{
				return m_inertia;
			}

			[[nodiscard]]
			float getInverseInertia() const
			{
				return m_inverse_inertia;
			}

		private:
			float m_mass = 1.0f, m_inverse_mass = 1.0f;
			float m_inertia = 1.0f, m_inverse_inertia = 1.0f;
		};

	} // namespace physics

} // namespace eg