#include "Graphics.hpp"
#include "include/Input/Input.hpp"
#include "include/Physics/RigidBody.hpp"
#include "include/Physics/World.hpp"
#include "include/Physics/Constraint.hpp"


class TheLayer : public Engine::Layer
{
public:
    TheLayer()
    {
        const auto width = (float)Engine::Application::getInstance().getWindow().getWidth();
        const auto height = (float)Engine::Application::getInstance().getWindow().getHeight();
        m_cam = std::make_unique<Engine::OrthographicCamera>((float)-width / 2, (float)width / 2, (float)-height / 2, (float)height / 2);

        circle = new Engine::Circle2D(50);

        constraint_circle = new Engine::Circle2D(500, 64);
        constraint_circle->setColor(Engine::Color::Black);
        constraint_circle->update();

        body.setCentroidPosition({ 300, 300 });
    }

    ~TheLayer()
    {
        delete circle;
        delete constraint_circle;
    }

    void onAttach() override {}
    void onDetach() override {}
    void onEvent(Engine::Event& e) override 
    {
        if (e.getEventType() == Engine::EventType::KeyPressed) 
        {
            const auto keycode = static_cast<Engine::KeyPressedEvent&>(e).GetKeyCode();
            if (keycode == Engine::Key::Escape) 
            {
                Engine::WindowCloseEvent close_signal{};
                Engine::Application::getInstance().onEvent(close_signal);
            }

            else if (keycode == Engine::Key::Space)
            {
                const auto v = body.getVelocity();
                body.setVelocity({ v.x, v.y + 1000.f });
            }
        }
    }

    void onUpdate() override
    { 
        proccessInput();

        static auto& program = Engine::DefaultShaderProgram::instance();
        program.use();
        program.setMat4("view", m_cam->getViewMatrix());
        program.setMat4("proj", m_cam->getProjMatrix());

        updateWorld();
        
        constraint_circle->draw();

        circle->setPosition(body.getCentroidPosition());
        circle->update();
        circle->draw();
    }

private:
    void proccessInput()
    {
    }

    bool isCollide(Engine::Shape* s1, Engine::Shape* s2)
    {
        for (int i = 0; i < 2; i++)
        {
            for (int a = 0; a < s1->getPointCount(); a++)
            {
                const int b = (a + 1) % s1->getPointCount();
                const auto pa = s1->getPoint(a);
                const auto pb = s1->getPoint(b);
                const Engine::Vector2f axisProj{-(pb.y - pa.y), pb.x - pa.x};

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

                if (min_r2 > max_r1 || max_r2 < min_r1)
                {
                    return false;
                }
            }

            std::swap(s1, s2);
        }
        return true;
    }

    void updateWorld()
    {
        constexpr int step = 10;
        constexpr auto dt = 1.0f / 170.0f;
        constexpr auto dtt = dt / float(step);
        for (int i = 0; i < step; i++)
        {
            body.applyForce({ 0, -2000 }); // apply gravity
            
            body.updateVelocity(dtt);

            applyImpulse(dtt);

            /*
            static Engine::Clock timer{};
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
        Engine::CircleConstraint constraint{};
        constraint.applyConstraint(body, dt);
    }

    std::unique_ptr<Engine::OrthographicCamera> m_cam;
    Engine::Circle2D* circle;
    Engine::Circle2D* constraint_circle;
    Engine::RigidBody body;
};

class SandBox : public Engine::Application
{
public:
    SandBox()
    {
        setClearColor(Engine::Color::Grey);
        pushLayer(new TheLayer{});
    }

    ~SandBox()
    {

    }
};

int main()
{
    Engine::Log::Init();

    SandBox app{};
    app.run();

    return 0;
}
