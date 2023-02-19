#pragma once

#include "include/Core/Vector.hpp"

#include <numbers>

namespace eg
{

	class RigidBody
	{
	public:
		Vector2f acceleration{}, linear_velocity{}, position{}, external_forces{};
		float angular_velocity = 0.0f, torque = 0.0f, rotate_radians = 0.0f;

		void update(const float dt)
		{
			acceleration = external_forces * m_mass_inv;
			linear_velocity += acceleration * dt;
			position = position + linear_velocity * dt;

			const auto angular_acc = torque * m_inertia_tensor_inv;
			angular_velocity += angular_acc * dt;
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
			linear_velocity += (impulse * m_mass_inv);
		}

		void applyTorque(const eg::Vector2f force, const eg::Vector2f point)
		{
			const auto r = point - position;
			torque += force.cross(r);
		}

		void setMass(const float mass)
		{
			m_mass = mass;
			m_mass_inv = 1.0f / mass;
		}

		[[nodiscard]]
		float getMass() const
		{
			return m_mass;
		}

		[[nodiscard]]
		float getInvMass() const
		{
			return m_mass_inv;
		}

		void setInertiaTensor(const float inertia)
		{
			m_inertia_tensor = inertia;
			m_inertia_tensor_inv = 1.0f / inertia;
		}

		void applyInertiaTensor(const eg::Vector2f r, const eg::Vector2f j)
		{
			angular_velocity += r.cross(j) * m_inertia_tensor_inv;
		}

		[[nodiscard]]
		float getInertiaTensor() const 
		{
			return m_inertia_tensor;
		}

		[[nodiscard]]
		float getInvInertiaTensor() const
		{
			return m_inertia_tensor_inv;
		}

	private:
		float m_mass = 1.0f, m_mass_inv = 1.0f;
		float m_inertia_tensor = 1.0f, m_inertia_tensor_inv = 1.0f;
	};

} // namespace eg