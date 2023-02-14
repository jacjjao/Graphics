#include "Graphics.hpp"
#include "include/Input/Input.hpp"
#include "include/Physics/RigidBody.hpp"
#include "include/Physics/World.hpp"
#include "include/Physics/Constraint.hpp"
#include "include/Physics/Utility.hpp"


class TheLayer : public eg::Layer
{
public:
    TheLayer()
    {
        const auto width = (float)eg::Application::getInstance().getWindow().getWidth();
        const auto height = (float)eg::Application::getInstance().getWindow().getHeight();
        m_cam = std::make_unique<eg::OrthographicCamera>((float)-width / 2, (float)width / 2, (float)-height / 2, (float)height / 2);

        circle = new eg::Circle2D(10);
        circle->setColor(eg::Color::Green);
        circle->update();

        constraint_circle = new eg::Circle2D(500, 64);
        constraint_circle->setColor(eg::Color::Black);
        constraint_circle->update();

        body.centroid_pos = { 300.f, 300.f };

        triangle = new eg::Circle2D(100, 4);
        triangle->setPosition({ 500.0f, 0.0f });
        triangle->setColor(eg::Color::Blue);
        triangle->update();

        rect = new eg::Rectangle2D(200, 300);
    }

    ~TheLayer()
    {
        delete circle;
        delete constraint_circle;

        delete triangle;
        delete rect;
    }

    void onAttach() override {}
    void onDetach() override {}
    void onEvent(eg::Event& e) override 
    {
        if (e.getEventType() == eg::EventType::KeyPressed) 
        {
            const auto keycode = static_cast<eg::KeyPressedEvent&>(e).GetKeyCode();
            if (keycode == eg::Key::Escape) 
            {
                eg::WindowCloseEvent close_signal{};
                eg::Application::getInstance().onEvent(close_signal);
            }

            else if (keycode == eg::Key::Space)
            {
                const auto v = body.linear_velocity;
                body.linear_velocity += { 0.0f, 1000.f };
            }
        }
    }

    void onUpdate() override
    { 
        proccessInput();

        static auto& program = eg::DefaultShaderProgram::instance();
        program.use();
        program.setMat4("view", m_cam->getViewMatrix());
        program.setMat4("proj", m_cam->getProjMatrix());

        if (auto col_data = isCollide(triangle, rect); col_data.depth != 0.0f)
        {
            const auto cp = genContactPoint(*triangle, *rect, col_data.normal);
            //EG_TRACE("Contact point: ({},{})", cp.x, cp.y);
            circle->setPosition(cp);
            circle->update();
            circle->draw();
            if (col_data.donor == rect)
                rect->translate(col_data.normal * col_data.depth);
            else 
                rect->translate(-col_data.normal * col_data.depth);
               
            //EG_TRACE("Depth: {}, Normal: ({},{})", col_data.depth, col_data.normal.x, col_data.normal.y);
        }

        triangle->draw();
        rect->draw();
        /*
        updateWorld();
        
        constraint_circle->draw();

        circle->setPosition(body.centroid_pos);
        circle->update();
        circle->draw();
        */
    }

private:
    void proccessInput()
    {
        constexpr float speed = 3.0f;

        if (eg::Input::isKeyPressed(eg::Key::W))
        {
            triangle->translate({ 0.0f, speed });
        }
        if (eg::Input::isKeyPressed(eg::Key::S))
        {
            triangle->translate({ 0.0f, -speed });
        }
        if (eg::Input::isKeyPressed(eg::Key::A))
        {
            triangle->translate({ -speed, 0.0f });
        }
        if (eg::Input::isKeyPressed(eg::Key::D))
        {
            triangle->translate({ speed, 0.0f });
        }
        if (eg::Input::isKeyPressed(eg::Key::E))
        {
            triangle->rotate(-1.0f);
        }
        if (eg::Input::isKeyPressed(eg::Key::Q))
        {
            triangle->rotate(1.0f);
        }
    }

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
            for (int a = 0; a < s1->getPointCount(); a++)
            {
                const int b = (a + 1) % s1->getPointCount();
                const auto pa = s1->getPoint(a);
                const auto pb = s1->getPoint(b);
                const auto axisProj = eg::Vector2f{-(pb.y - pa.y), pb.x - pa.x}.normalize();

                auto min_r1 =  std::numeric_limits<float>::infinity();
                auto max_r1 = -std::numeric_limits<float>::infinity();
                for (int p = 0; p < s1->getPointCount(); p++) 
                {
                    const auto q = s1->getPoint(p) * axisProj;
                    min_r1 = std::min(min_r1, q);
                    max_r1 = std::max(max_r1, q);
                }

                auto min_r2 =  std::numeric_limits<float>::infinity();
                auto max_r2 = -std::numeric_limits<float>::infinity();
                for (int p = 0; p < s2->getPointCount(); p++)
                {
                    const auto q = s2->getPoint(p) * axisProj;
                    min_r2 = std::min(min_r2, q);
                    max_r2 = std::max(max_r2, q);
                }

                if (eg::certainGreater(min_r2, max_r1) or eg::certainLess(max_r2, min_r1))
                {
                    return {};
                }

                const float penDepth = std::min(max_r1, max_r2) - std::max(min_r1, min_r2);
                if (const float x = axisProj * pvec, y = result.normal * pvec;
                    eg::nearlyEqual(penDepth, result.depth) and eg::certainGreater(x, y))
                {
                    result.donor = s2;
                    result.receptor = s1;
                    result.normal = axisProj;
                }
                else if (eg::certainLess(penDepth, result.depth))
                {
                    result.donor = s2;
                    result.receptor = s1;
                    result.depth = penDepth;
                    result.normal = axisProj;
                }
            }

            std::swap(s1, s2);
        }
        return result;
    }

    eg::Vector2f genContactPoint(eg::Shape& s1, eg::Shape& s2, eg::Vector2f normal)
    {
        float dist = -std::numeric_limits<float>::infinity();
        eg::Vector2f farthestP{};
        for (size_t n = s1.getPointCount(), i = 0; i < n; i++)
        {
            const auto p = s1.getPoint(i);
            const auto proj = p * normal;
            if (proj > dist)
            {
                dist = proj;
                farthestP = p;
            }
        }
        return farthestP;
    }

    void updateWorld()
    {
        constexpr int step = 10;
        constexpr auto dt = 1.0f / 170.0f;
        constexpr auto dtt = dt / float(step);
        for (int i = 0; i < step; i++)
        {
            body.external_forces = { 0, -2000 }; // apply gravity
            
            body.updateVelocity(dtt);

            applyImpulse(dtt);

            /*
            static eg::Clock timer{};
            if (timer.getElapsedTime().asMilliseconds() >= 800)
            {
                const auto v = body.getVelocity();
                const auto h = body.getCentroidPosition().y;
                EG_TRACE("{}", v * v * 0.5f + 2000.f * std::abs(h));
                timer.restart();
            }
            */

            body.updatePosition(dtt);
        }
    }

    void applyImpulse(const float dt)
    {
        /*
        const auto pos = body.getCentroidPosition();
        if (pos.y >= -450.f)
            return;

        const float bias = -450.f - pos.y;
        const float lambda = -body.getVelocity().y / dt - body.getAcceleration().y + bias / dt;
        body.applyForce({ 0, lambda });
        */
        /*
        const auto pos = body.getCentroidPosition();
        if (pos.x * pos.x + pos.y * pos.y <= 450.f * 450.f)
            return;

        const auto v = body.getVelocity() + body.getAcceleration() * dt;
        const float lambda = -(pos.x * v.x + pos.y * v.y) / (pos * pos * dt);
        body.applyForce({ pos.x * lambda, pos.y * lambda });
        */
        eg::CircleConstraint constraint{};
        constraint.applyConstraint(body, dt);
    }

    std::unique_ptr<eg::OrthographicCamera> m_cam;
    eg::Circle2D* circle;
    eg::Circle2D* constraint_circle;
    eg::RigidBody body;

    eg::Circle2D* triangle;
    eg::Rectangle2D* rect;
};

class SandBox : public eg::Application
{
public:
    SandBox()
    {
        setClearColor(eg::Color::Grey);
        pushLayer(new TheLayer{});
    }

    ~SandBox()
    {

    }
};

int main()
{
    eg::Log::Init();

    SandBox app{};
    app.run();

    return 0;
}
