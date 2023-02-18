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

        std::optional<CollisionData> isCollide(Polygon& s1, Polygon& s2)
        {
            CollisionData result{};
            result.depth =  std::numeric_limits<float>::infinity();
            auto max_dot = -std::numeric_limits<float>::infinity();
            for (int i = 0; i < 2; i++)
            {
                const auto pvec = s2.poly->getPosition() - s1.poly->getPosition();
                for (size_t a = 0; a < s1.vertices.size(); a++)
                {
                    const size_t b = (a + 1) % s1.vertices.size();
                    const auto pa = s1.vertices[a];
                    const auto pb = s1.vertices[b];
                    const auto axisProj = eg::Vector2f{ -(pb.y - pa.y), pb.x - pa.x }.normalize();

                    auto min_r1 =  std::numeric_limits<float>::infinity();
                    auto max_r1 = -std::numeric_limits<float>::infinity();
                    for (const auto p : s1.vertices)
                    {
                        const auto q = p * axisProj;
                        min_r1 = std::min(min_r1, q);
                        max_r1 = std::max(max_r1, q);
                    }

                    auto min_r2 =  std::numeric_limits<float>::infinity();
                    auto max_r2 = -std::numeric_limits<float>::infinity();
                    for (const auto p : s2.vertices)
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
                        result.donor    = &s2;
                        result.receptor = &s1;
                        result.depth    = std::min(penDepth, result.depth);
                        result.normal   = axisProj;

                        max_dot = axisProj * pvec;
                    }
                    else if (penDepth < result.depth)
                    {
                        result.donor    = &s2;
                        result.receptor = &s1;
                        result.depth    = penDepth;
                        result.normal   = axisProj;

                        max_dot = axisProj * pvec;
                    }
                }
                std::swap(s1, s2);
            }
            return result;
        }

        std::pair<eg::Vector2f, eg::Vector2f> findSignificantFace(std::span<eg::Vector2f> vertices, eg::Vector2f normal)
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
            eg::Vector2f p1, p2;
            int cnt;
        };

        PenetratePoints findPentratePoint(const CollisionData col_data, const std::pair<eg::Vector2f, eg::Vector2f> sig_face)
        {
            const auto pointSegDist = [](const eg::Vector2f p, const eg::Vector2f a, const eg::Vector2f b) {
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
                const auto dist = pointSegDist(p, sig_face.first, sig_face.second);

                if (eg::nearlyEqual(dist, minDist))
                {
                    pp.cnt = 2;
                    pp.p2 = p;
                    minDist = std::min(dist, minDist);
                }
                else if (dist < minDist)
                {
                    pp.cnt = 1;
                    pp.p1 = p;
                    minDist = dist;
                }
            }
            return pp;
        }

        void resolveCollision(RigidBody& receptor, RigidBody& donor, const eg::Vector2f col_normal, const float depth, const float dt)
        {
            static constexpr float bounciness = 1.0f;

            const auto vab = receptor.linear_velocity - donor.linear_velocity;
            const auto impulseMag = vab * col_normal * (-1.0f - bounciness) / (col_normal * col_normal * (donor.getInvMass() + receptor.getInvMass()));

            receptor.applyImpulse(col_normal * impulseMag);
            donor.applyImpulse(-col_normal * impulseMag);
            
            const float mass_sum_inv = 1.0f / (receptor.getMass() + donor.getMass());
            receptor.centroid_pos = receptor.centroid_pos - col_normal * (depth * receptor.getMass() * mass_sum_inv);
            donor.centroid_pos = donor.centroid_pos + col_normal * (depth * donor.getMass() * mass_sum_inv);
        }
        /*

        struct Face
        {
            eg::Vector2f a, b;
        };

        struct CollisionFaces
        {
            Face inc_face, ref_face;
        };

        CollisionFaces findRefFace(const Face f1, const Face f2, eg::Vector2f col_normal)
        {
            if (std::abs((f1.a - f1.b) * col_normal) < std::abs((f2.a - f2.b) * col_normal))
            {
                return { f2, f1 };
            }
            return { f1, f2 };
        }

        struct Line
        {
            eg::Vector2f normal, point;
        };

        std::pair<eg::Vector2f, eg::Vector2f> clipping(Face inc, Face ref, eg::Vector2f col_normal)
        {
            if (ref.a.x > ref.b.x or (ref.a.x == ref.b.x and ref.a.y > ref.b.y))
            {
                std::swap(ref.a, ref.b);
            }

            const auto inc_direc = inc.b - inc.a;
            const Line inc_l = { eg::Vector2f{ -inc_direc.y, inc_direc.x }, inc.a };

            const auto ref_direc = ref.b - ref.a;
            const Line ref_l = { eg::Vector2f{ -ref_direc.y, ref_direc.x }, ref.a };

            const auto clip_line_normal = ref.b - ref.a;
            const auto c1 = -clip_line_normal * ref.a;
            const auto c2 = -clip_line_normal * ref.b; // ax+by+c=0

            const auto getIntersectPoint = [](const Line l1, const Line l2) {
                const auto c1 = -l1.normal * l1.point;
                const auto c2 = -l2.normal * l2.point;

                return eg::Vector2f{
                    (l1.normal.y * c2 - l2.normal.y * c1) / (l1.normal.x * l2.normal.y - l2.normal.x * l1.normal.y),
                    (l2.normal.x * c1 - l1.normal.x * c2) / (l1.normal.x * l2.normal.y - l2.normal.x * l1.normal.y)
                };
            };

            if (clip_line_normal * inc.a + c1 < 0.0f) // if inc.a need to clip
            {
                inc.a = getIntersectPoint({ clip_line_normal, ref.a }, inc_l);
            }
            else if (clip_line_normal * inc.b + c1 < 0.0f)
            {
                inc.b = getIntersectPoint({ clip_line_normal, ref.a }, inc_l);
            }

            if (clip_line_normal * inc.a + c2 > 0.0f)
            {
                inc.a = getIntersectPoint({ clip_line_normal, ref.b }, inc_l);
            }
            else if (clip_line_normal * inc.b + c2 > 0.0f)
            {
                inc.b = getIntersectPoint({ clip_line_normal, ref.b }, inc_l);
            }

            // final clipping
            const auto c_rf = -ref_l.normal * ref.a;
            if (inc.a * -col_normal >= inc.b * -col_normal)
            {
                return { inc.a, inc.a };
            }
            return { inc.b, inc.b };
        }
        */
    } // namespace physics

} // namespace eg