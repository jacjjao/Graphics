#pragma once

namespace eg
{
    namespace physics
    {

        struct CollisionData
        {
            eg::Shape* donor;
            eg::Shape* receptor;
            eg::Vector2f normal;
            float depth;
        };

        CollisionData isCollide(eg::Shape* s1, eg::Shape* s2)
        {
            CollisionData result{};
            result.depth = std::numeric_limits<float>::infinity();
            for (int i = 0; i < 2; i++)
            {
                const auto pvec = s2->getPosition() - s1->getPosition();
                float max_dot = -std::numeric_limits<float>::infinity();
                for (int a = 0; a < s1->getPointCount(); a++)
                {
                    const int b = (a + 1) % s1->getPointCount();
                    const auto pa = s1->getPoint(a);
                    const auto pb = s1->getPoint(b);
                    const auto axisProj = eg::Vector2f{ -(pb.y - pa.y), pb.x - pa.x }.normalize();

                    auto min_r1 = std::numeric_limits<float>::infinity();
                    auto max_r1 = -std::numeric_limits<float>::infinity();
                    for (int p = 0; p < s1->getPointCount(); p++)
                    {
                        const auto q = s1->getPoint(p) * axisProj;
                        min_r1 = std::min(min_r1, q);
                        max_r1 = std::max(max_r1, q);
                    }

                    auto min_r2 = std::numeric_limits<float>::infinity();
                    auto max_r2 = -std::numeric_limits<float>::infinity();
                    for (int p = 0; p < s2->getPointCount(); p++)
                    {
                        const auto q = s2->getPoint(p) * axisProj;
                        min_r2 = std::min(min_r2, q);
                        max_r2 = std::max(max_r2, q);
                    }

                    if (min_r2 > max_r1 or max_r2 < min_r1)
                    {
                        return {};
                    }

                    const float penDepth = std::min(max_r1, max_r2) - std::max(min_r1, min_r2);
                    if (eg::nearlyEqual(penDepth, result.depth) and axisProj * pvec > max_dot)
                    {
                        result.donor = s2;
                        result.receptor = s1;
                        result.depth = std::min(penDepth, result.depth);
                        result.normal = axisProj;
                        max_dot = axisProj * pvec;
                    }
                    else if (penDepth < result.depth)
                    {
                        result.donor = s2;
                        result.receptor = s1;
                        result.depth = penDepth;
                        result.normal = axisProj;
                        max_dot = axisProj * pvec;
                    }
                }

                std::swap(s1, s2);
            }
            return result;
        }

        std::pair<eg::Vector2f, eg::Vector2f> findSignificantFace(eg::Shape& sp, eg::Vector2f normal)
        {
            size_t farthestPidx = 0;
            float dist = -std::numeric_limits<float>::infinity();
            for (size_t n = sp.getPointCount(), i = 0; i < n; i++)
            {
                const auto p = sp.getPoint(i);
                const auto proj = p * normal;
                if (proj > dist)
                {
                    dist = proj;
                    farthestPidx = i;
                }
            }

            eg::Vector2f sigFace{};
            const auto farthestP = sp.getPoint(farthestPidx);
            float minSlope = std::numeric_limits<float>::infinity();
            for (size_t n = sp.getPointCount(), i = 0; i < n; i++)
            {
                if (i == farthestPidx)
                {
                    continue;
                }
                const auto face = sp.getPoint(i) - farthestP;
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
            for (size_t n = col_data.donor->getPointCount(), i = 0; i < n; i++) 
            {
                const auto p = col_data.donor->getPoint(i);
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

        void resolveCollision(RigidBody& a, RigidBody& b, const eg::Vector2f col_normal, const float depth)
        {
            const auto vab = b.linear_velocity - a.linear_velocity;
            const auto impulse = -vab * col_normal / (col_normal * col_normal * (1.0f / a.mass + 1.0f / b.mass));

            a.linear_velocity = a.linear_velocity - col_normal * (impulse / a.mass);
            b.linear_velocity = b.linear_velocity + col_normal * (impulse / b.mass);

            a.centroid_pos = a.centroid_pos - col_normal * (depth / 2.0f);
            b.centroid_pos = b.centroid_pos + col_normal * (depth / 2.0f);
        }

    } // namespace physics

} // namespace eg