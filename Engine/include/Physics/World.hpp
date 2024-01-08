#pragma once

#include "include/Core/Vector.hpp"
#include "RigidBody.hpp"
#include "include/Renderer/Rectangle2D.hpp"
#include "include/Renderer/Circle2D.hpp"

#include <memory>
#include <vector>

namespace eg
{
	namespace physics
	{
		class World
		{
		public:
			void setGravity(const Vector2f gravity) { m_gravity = gravity; }

			void update()
			{
				constexpr int step = 10;
				for (int i = 0; i < step; i++)
				{
					getAllpoint();
					detectCollision();
					applyGravity();
					applyConstraint();
					update();
				}
			}

			void addBody(const RigidBody& rd, eg::Color color = eg::Color::White)
			{
				m_bodies.push_back(rd);
				switch (rd.type)
				{
				case RigidBodyType::Box:
				{
					auto p = std::make_unique<eg::Rectangle2D>(rd.width, rd.height);
					p->setColor(color);
					p->update();
					m_shapes.emplace_back(p.release());
					break;
				}
				case RigidBodyType::Circle:
				{
					auto p = std::make_unique<eg::Circle2D>(rd.radius);
					p->setColor(color);
					p->update();
					m_shapes.emplace_back(p.release());
					break;
				}
				}
			}

		private:
			void getAllpoint()
			{

			}

			void detectCollision()
			{

			}

			void applyGravity()
			{

			}

			void applyConstraint()
			{
			}

			Vector2f m_gravity;
			std::vector<RigidBody> m_bodies;
			std::vector<std::unique_ptr<eg::Shape>> m_shapes;
		};
	} // physics

} // namespace eg
