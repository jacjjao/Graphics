#pragma once

namespace eg
{
    namespace physics
    {
        struct Polygon
        {
            eg::Shape* poly = nullptr;
            std::vector<eg::Vector2f> vertices{};
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
            std::array<eg::Vector2f, 2> p;
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
            size_t cnt;
        };

        PenetratePoints findPentratePoint(LineSegment inc_face, LineSegment ref_face)
        {
            const auto pointToSegDist = [](const eg::Vector2f p, const LineSegment line) {
                const auto& a = line.p[0];
                const auto& b = line.p[1];
                
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
            for (size_t i = 0; i < 2; i++)
            {
                for (const auto& p : inc_face.p)
                {
                    const auto dist = pointToSegDist(p, ref_face);

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
                std::swap(inc_face, ref_face);
            }
            return pp;
        }
        
        void resolveCollision(RigidBody& bodyA, RigidBody& bodyB, const PenetratePoints pp, const eg::Vector2f n, const float depth)
        { 
            constexpr float bounciness = 0.2f; // ¼u©Ê±`¼Æ
            constexpr float static_friction = 0.6f;
            constexpr float dynamic_friction = 0.4f;

            std::array<eg::Vector2f, 2> impulse_list{};
            std::array<eg::Vector2f, 2> friction_impulse{};
            std::array<eg::Vector2f, 2> ra_list{};
            std::array<eg::Vector2f, 2> rb_list{};
            std::array<float, 2> j_list{};

            if (!bodyA.is_static and !bodyB.is_static)
            {
                for (size_t i = 0; i < pp.cnt; i++)
                {
                    const auto ra = pp.cp[i] - bodyA.position;
                    const auto rb = pp.cp[i] - bodyB.position;

                    const eg::Vector2f ra_prep = { -ra.y, ra.x };
                    const eg::Vector2f rb_prep = { -rb.y, rb.x };

                    ra_list[i] = ra;
                    rb_list[i] = rb;

                    const auto relativeVelMag =
                        ((bodyB.linear_velocity + bodyB.angular_velocity * rb_prep) -
                         (bodyA.linear_velocity + bodyA.angular_velocity * ra_prep)) * n;

                    if (relativeVelMag > 0.0f)
                    {
                        continue;
                    }

                    const auto ra_cross_n = ra.cross(n);
                    const auto rb_cross_n = rb.cross(n);
                    const auto denom = bodyA.getInverseMass() + bodyB.getInverseMass() + 
                        (ra_cross_n * ra_cross_n * bodyA.getInverseInertia()) + 
                        (rb_cross_n * rb_cross_n * bodyB.getInverseInertia());

                    auto j = -(1.0f + bounciness) * relativeVelMag;
                    j /= denom;
                    if (pp.cnt == 2)
                        j *= 0.5f;

                    j_list[i] = j;
                    impulse_list[i] = n * j;
                }
            }
            else
            {
                for (size_t i = 0; i < pp.cnt; i++)
                {
                    float relativeVelMag = 0.0f;
                    float denom = 0.0f;
                    if (bodyA.is_static)
                    {
                        const auto rb = pp.cp[i] - bodyB.position;
                        rb_list[i] = rb;
                        const eg::Vector2f rb_prep = { -rb.y, rb.x };
                        const auto rb_cross_n = rb.cross(n);
                        relativeVelMag = (bodyB.linear_velocity + rb_prep * bodyB.angular_velocity) * n;
                        denom = bodyB.getInverseMass() + rb_cross_n * rb_cross_n * bodyB.getInverseInertia();
                    }
                    else
                    {
                        const auto ra = pp.cp[i] - bodyA.position;
                        ra_list[i] = ra;
                        const eg::Vector2f ra_prep = { -ra.y, ra.x };
                        const auto ra_cross_n = ra.cross(n);
                        relativeVelMag = -(bodyA.linear_velocity + ra_prep * bodyA.angular_velocity) * n;
                        denom = bodyA.getInverseMass() + ra_cross_n * ra_cross_n * bodyA.getInverseInertia();
                    }

                    if (relativeVelMag > 0.0f)
                    {
                        continue;
                    }

                    auto j = -(1.0f + bounciness) * relativeVelMag / denom;
                    if (pp.cnt == 2)
                        j *= 0.5f;

                    j_list[i] = j;
                    impulse_list[i] = n * j;
                }
            }

            // friction
            if (!bodyA.is_static and !bodyB.is_static)
            {
                for (size_t i = 0; i < pp.cnt; i++)
                {
                    const auto ra = ra_list[i];
                    const auto rb = rb_list[i];

                    const eg::Vector2f ra_prep = { -ra.y, ra.x };
                    const eg::Vector2f rb_prep = { -rb.y, rb.x };

                    const auto relativeVel =
                        (bodyB.linear_velocity + bodyB.angular_velocity * rb_prep) -
                        (bodyA.linear_velocity + bodyA.angular_velocity * ra_prep);

                    auto tangent = relativeVel - (relativeVel * n) * n;

                    if (eg::nearlyEqual(tangent.x, 0.0f) and eg::nearlyEqual(tangent.y, 0.0f))
                    {
                        continue;
                    }
                    else
                    {
                        tangent = tangent.normalize();
                    }

                    const auto ra_prep_dot_t = ra_prep * tangent;
                    const auto rb_prep_dot_t = rb_prep * tangent;
                    const auto denom = bodyA.getInverseMass() + bodyB.getInverseMass() +
                        (ra_prep_dot_t * ra_prep_dot_t) * bodyA.getInverseInertia() +
                        (rb_prep_dot_t * rb_prep_dot_t) * bodyB.getInverseInertia();

                    auto jt = -relativeVel * tangent;
                    jt /= denom;
                    if (pp.cnt == 2)
                        jt *= 0.5f;

                    eg::Vector2f f_impulse{};
                    if (std::abs(jt) <= j_list[i] * static_friction)
                    {
                        f_impulse = jt * tangent;
                    }
                    else
                    {
                        f_impulse = -j_list[i] * tangent * dynamic_friction;
                    }

                    friction_impulse[i] = f_impulse;
                }
            }
            else
            {
                for (size_t i = 0; i < pp.cnt; i++)
                {
                    const auto ra = ra_list[i];
                    const auto rb = rb_list[i];

                    const eg::Vector2f ra_prep = { -ra.y, ra.x };
                    const eg::Vector2f rb_prep = { -rb.y, rb.x };

                    const auto relativeVel =
                        (bodyB.linear_velocity + bodyB.angular_velocity * rb_prep) -
                        (bodyA.linear_velocity + bodyA.angular_velocity * ra_prep);

                    auto tangent = relativeVel - (relativeVel * n) * n;

                    if (eg::nearlyEqual(tangent.x, 0.0f) and eg::nearlyEqual(tangent.y, 0.0f))
                    {
                        continue;
                    }
                    else
                    {
                        tangent = tangent.normalize();
                    }

                    const auto ra_prep_dot_t = ra_prep * tangent;
                    const auto rb_prep_dot_t = rb_prep * tangent;

                    float denom = 0.0f;
                    if (bodyA.is_static)
                    {
                        denom = bodyB.getInverseMass() + (rb_prep_dot_t * rb_prep_dot_t) * bodyB.getInverseInertia();
                    }
                    else
                    {
                        denom = bodyA.getInverseMass() + (ra_prep_dot_t * ra_prep_dot_t) * bodyA.getInverseInertia();
                    }

                    auto jt = -relativeVel * tangent;
                    jt /= denom;
                    if (pp.cnt == 2)
                        jt *= 0.5f;

                    eg::Vector2f f_impulse{};
                    if (std::abs(jt) <= j_list[i] * static_friction)
                    {
                        f_impulse = jt * tangent;
                    }
                    else
                    {
                        f_impulse = -j_list[i] * tangent * dynamic_friction;
                    }

                    friction_impulse[i] = f_impulse;
                }
            }
            for (size_t i = 0; i < pp.cnt; i++)
            {
                if (!bodyA.is_static)
                {
                    bodyA.applyImpulse(-impulse_list[i]);
                    bodyA.applyInertiaTensor(ra_list[i], -impulse_list[i]);

                    bodyA.applyImpulse(-friction_impulse[i]);
                    bodyA.applyInertiaTensor(ra_list[i], -friction_impulse[i]);
                }

                if (!bodyB.is_static)
                {
                    bodyB.applyImpulse(impulse_list[i]);
                    bodyB.applyInertiaTensor(rb_list[i], impulse_list[i]);

                    bodyB.applyImpulse(friction_impulse[i]);
                    bodyB.applyInertiaTensor(rb_list[i], friction_impulse[i]);
                }
            }
            if (bodyA.is_static)
            {
                bodyB.position = bodyB.position + n * depth;
            }
            else if (bodyB.is_static)
            {
                bodyA.position = bodyA.position - n * depth;
            }
            else
            {
                const float mass_sum_inv = 1.0f / (bodyA.getMass() + bodyB.getMass());
                bodyA.position = bodyA.position - n * (depth * bodyA.getMass() * mass_sum_inv);
                bodyB.position = bodyB.position + n * (depth * bodyB.getMass() * mass_sum_inv);
            }
        }
        

    } // namespace physics

} // namespace eg