#include "Graphics.hpp"
#include "include/Input/Input.hpp"
#include "include/Physics/RigidBody.hpp"
#include "include/Physics/World.hpp"

Engine::Vector2f screenPointToGL(const Engine::Vector2f vec, const float scr_width, const float scr_height)
{
    const auto half_scr_width = scr_width / 2.0f;
    const auto half_scr_height = scr_height / 2.0f;
    return {
        vec.x - half_scr_width,
        half_scr_height - vec.y
    };
}

class TheLayer : public Engine::Layer
{
public:
    TheLayer()
    {
        const auto width = (float)Engine::Application::getInstance().getWindow().getWidth();
        const auto height = (float)Engine::Application::getInstance().getWindow().getHeight();
        m_cam = std::make_unique<Engine::OrthographicCamera>((float)-width / 2, (float)width / 2, (float)-height / 2, (float)height / 2);

        rect = new Engine::Rectangle2D(100, 100);
        rect->setColor(Engine::Color::Cyan);
        rect->update();

        circle = new Engine::Circle2D(100, 3);
        circle->setPosition({ 100, 500 });
        circle->update();
    }

    ~TheLayer()
    {
        delete circle;
        delete rect;
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
        }
    }

    void onUpdate() override
    { 
        constexpr auto speed = 1.0f;
        if (Engine::Input::isKeyPressed(Engine::Key::W)) 
        {
            circle->translate({ 0.0f, speed });
        }
        if (Engine::Input::isKeyPressed(Engine::Key::S)) 
        {
            circle->translate({ 0.0f, -speed });
        }
        if (Engine::Input::isKeyPressed(Engine::Key::A)) 
        {
            circle->translate({ -speed, 0.0f });
        }
        if (Engine::Input::isKeyPressed(Engine::Key::D)) 
        {
            circle->translate({ speed, 0.0f });
        }
        if (Engine::Input::isKeyPressed(Engine::Key::E)) 
        {
            circle->rotate(-speed);
        }
        if (Engine::Input::isKeyPressed(Engine::Key::Q)) 
        {
            circle->rotate(speed);
        }
        static float scale = 1.0f;
        if (Engine::Input::isKeyPressed(Engine::Key::Space))
        {
            scale *= 1.1f;
            circle->scale(scale);
        }
        if (Engine::Input::isKeyPressed(Engine::Key::LeftShift))
        {
            scale *= 0.9f;
            circle->scale(scale);
        }

        if (isCollide(circle, rect))
        {
            circle->setColor(Engine::Color::Red);
            circle->update();
        }
        else
        {
            circle->setColor(Engine::Color::Blue);
            circle->update();
        }

        static auto& program = Engine::DefaultShaderProgram::instance();
        program.use();
        program.setMat4("view", m_cam->getViewMatrix());
        program.setMat4("proj", m_cam->getProjMatrix());

        rect->draw();
        circle->draw();

        /*
        static Engine::Clock clock;
        if (clock.getElapsedTime().asSeconds() >= 1.0) {
            EG_TRACE("Rectangle: ({},{})", rect->getPosition().x, rect->getPosition().y);
            EG_TRACE("Circle: ({},{})", circle->getPosition().x, circle->getPosition().y);
            clock.restart();
        }
        */
    }

private:
    bool isCollide(Engine::Shape* s1, Engine::Shape* s2)
    {
        //for (int i = 0; i < 2; i++)
        //{
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

            //std::swap(s1, s2);
        //}
        return true;
    }

    std::unique_ptr<Engine::OrthographicCamera> m_cam;
    Engine::Rectangle2D* rect;
    Engine::Circle2D* circle;
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
