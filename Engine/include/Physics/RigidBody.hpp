#pragma once

#include "include/Core/Vector.hpp"

#include <numbers>

namespace eg
{

	namespace physics
	{

		enum class RigidBodyType
		{
			Box, Circle
		};

		class RigidBody
		{
		public:
			Vector2f linear_velocity{}, position{}, external_forces = {};
			float angular_velocity = 0.0f, rotate_radians = 0.0f;
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
				linear_velocity += acceleration * dt;
				position = position + linear_velocity * dt;

				rotate_radians += angular_velocity * dt;
				/*
				constexpr auto two_pi	  = 2.0f * std::numbers::pi_v<float>;
				constexpr auto two_pi_inv = 1.0f / two_pi;
				const auto m = static_cast<int>(rotate_radians * two_pi_inv);
				rotate_radians -= m * two_pi; // clamp it in [0,2pi]
				*/
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

			void applyInertiaTensor(const eg::Vector2f r, const eg::Vector2f j)
			{
				angular_velocity += r.cross(j) * m_inverse_inertia;
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