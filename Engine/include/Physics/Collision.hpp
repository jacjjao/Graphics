#pragma once

namespace eg
{
    namespace physics
    {
        struct Polygon
        {
            eg::Shape* poly;
            std::vector<eg::Vector2f> vertices;
        };

        struct CollisionData
        {
            Polygon* donor;
            Polygon* receptor;
            eg::Vector2f normal;
            float depth;
        };

        std::optional<CollisionData> isCollide(Polygon* s1, Polygon* s2)
        {
            CollisionData result{};
            result.depth =  std::numeric_limits<float>::infinity();
            auto max_dot = -std::numeric_limits<float>::infinity();
            for (int i = 0; i < 2; i++)
            {
                const auto pvec = s2->poly->getPosition() - s1->poly->getPosition();
                for (size_t a = 0; a < s1->vertices.size(); a++)
                {
                    const size_t b = (a + 1) % s1->vertices.size();
                    const auto pa = s1->vertices[a];
                    const auto pb = s1->vertices[b];
                    const auto axisProj = eg::Vector2f{ -(pb.y - pa.y), pb.x - pa.x }.normalize();

                    auto min_r1 =  std::numeric_limits<float>::infinity();
                    auto max_r1 = -std::numeric_limits<float>::infinity();
                    for (const auto p : s1->vertices)
                    {
                        const auto q = p * axisProj;
                        min_r1 = std::min(min_r1, q);
                        max_r1 = std::max(max_r1, q);
                    }

                    auto min_r2 =  std::numeric_limits<float>::infinity();
                    auto max_r2 = -std::numeric_limits<float>::infinity();
                    for (const auto p : s2->vertices)
                    {
                        const auto q = p * axisProj;
                        min_r2 = std::min(min_r2, q);
                        max_r2 = std::max(max_r2, q);
                    }

                    if (min_r2 > max_r1 or max_r2 < min_r1)
                    {
                        return std::nullopt;
                    }

                    const auto penDepth = std::min(max_r1, max_r2) - std::max(min_r1, min_r2);
                    const auto axisDot  = axisProj * pvec;
                    if (eg::nearlyEqual(penDepth, result.depth) and axisDot > max_dot)
                    {
                        result.donor    = s2;
                        result.receptor = s1;
                        result.depth    = std::min(penDepth, result.depth);
                        result.normal   = axisProj;

                        max_dot = axisDot;
                    }
                    else if (penDepth < result.depth)
                    {
                        result.donor    = s2;
                        result.receptor = s1;
                        result.depth    = penDepth;
                        result.normal   = axisProj;

                        max_dot = axisDot;
                    }
                }
                std::swap(s1, s2);
            }
            return result;
        }

        struct LineSegment
        {
            eg::Vector2f a, b;
        };

        LineSegment findSignificantFace(std::span<eg::Vector2f> vertices, eg::Vector2f normal)
        {
            size_t farthestPidx = 0;
            float dist = -std::numeric_limits<float>::infinity();
            for (size_t i = 0; i < vertices.size(); i++)
            {
                const auto proj = vertices[i] * normal;
                if (proj > dist)
                {
                    dist = proj;
                    farthestPidx = i;
                }
            }

            eg::Vector2f sigFace{};
            const auto farthestP = vertices[farthestPidx];
            float minSlope = std::numeric_limits<float>::infinity();
            for (size_t i = 0; i < vertices.size(); i++)
            {
                if (i == farthestPidx)
                {
                    continue;
                }
                const auto face = vertices[i] - farthestP;
                if (const auto slope = std::abs(face * normal); slope < minSlope)
                {
                    sigFace = face;
                    minSlope = slope;
                }
            }
            return { farthestP, farthestP + sigFace };
        }

        struct PenetratePoints
        {
            std::array<eg::Vector2f, 2> cp;
            int cnt;
        };

        PenetratePoints findPentratePoint(const CollisionData col_data, const LineSegment sig_face)
        {
            const auto pointToSegDist = [](const eg::Vector2f p, const LineSegment line) {
                const auto& [a, b] = line;
                
                const auto ab = b - a;
                const auto ap = p - a;

                const auto proj = ap * ab;
                const auto abLenSq = ab.lengthSquared();
                const auto d = proj / abLenSq;

                eg::Vector2f cp{};
                if (d <= 0)
                {
                    cp = a;
                }
                else if (d >= 1)
                {
                    cp = b;
                }
                else
                {
                    cp = a + ab * d;
                }
                return (p - cp).length();
            };

            PenetratePoints pp{};
            auto minDist = std::numeric_limits<float>::infinity();
            for (size_t i = 0; i < col_data.donor->vertices.size(); i++) 
            {
                const auto p = col_data.donor->vertices[i];
                const auto dist = pointToSegDist(p, sig_face);

                if (eg::nearlyEqual(dist, minDist))
                {
                    pp.cnt = 2;
                    pp.cp[1] = p;
                    minDist = std::min(dist, minDist);
                }
                else if (dist < minDist)
                {
                    pp.cnt = 1;
                    pp.cp[0] = p;
                    minDist = dist;
                }
            }
            return pp;
        }

        void resolveCollision(RigidBody& bodyA, RigidBody& bodyB, const PenetratePoints pp, const eg::Vector2f n, const float depth)
        { 
            constexpr float bounciness = 1.0f; // ¼u©Ê±`¼Æ
            for (int i = 0; i < pp.cnt; i++)
            {
                const auto ra = pp.cp[i] - bodyA.position;
                const auto rb = pp.cp[i] - bodyB.position;
                const eg::Vector2f pa_perp{ -ra.y, ra.x };
                const eg::Vector2f pb_perp{ -rb.y, rb.x };
                const auto ang_vel_a = pa_perp * bodyA.angular_velocity;
                const auto ang_vel_b = pb_perp * bodyB.angular_velocity;

                const auto vab = (bodyB.linear_velocity + ang_vel_b) - (bodyA.linear_velocity + ang_vel_a);
                const auto relativeVelMag = vab * n;
                
                if (relativeVelMag > 0.0f)
                {
                    continue;
                }
                
                const auto rap_dot_n = pa_perp * n;
                const auto rbp_dot_n = pb_perp * n;
                const auto denom = bodyA.getInvMass() + bodyB.getInvMass();  

                auto j = relativeVelMag * -(1.0f + bounciness);
                j /= denom;
                j /= static_cast<float>(pp.cnt);

                const auto impulse = n * j;
                bodyA.applyImpulse(-impulse);
                // receptor.applyInertiaTensor(ras[i], -impulses[i]);

                bodyB.applyImpulse(impulse);
                // donor.applyInertiaTensor(rbs[i], impulses[i]);
            }
            /*
            const float mass_sum_inv = 1.0f / (bodyA.getMass() + bodyB.getMass());
            bodyA.position = bodyA.position - n * (depth * bodyA.getMass() * mass_sum_inv);
            bodyB.position = bodyB.position + n * (depth * bodyB.getMass() * mass_sum_inv);
            */
        }

    } // namespace physics

} // namespace eg