#include "Graphics.hpp"
#include "include/Input/Input.hpp"
#include "include/Physics/RigidBody.hpp"
#include "include/Physics/World.hpp"
#include "include/Physics/Constraint.hpp"
#include "include/Physics/Utility.hpp"
#include "include/Physics/Collision.hpp"


eg::Vector2f ScreenToGL(const eg::Vector2f pos)
{
    static const auto half_width = float(eg::Application::getInstance().getWindow().getWidth() / 2);
    static const auto half_height = float(eg::Application::getInstance().getWindow().getHeight() / 2);
    return {
        (pos.x - half_width),
        (half_height - pos.y)
    };
}

class TheLayer : public eg::Layer
{
public:
    TheLayer()
    {
        eg::Renderer2D::Init();

        const auto width = (float)eg::Application::getInstance().getWindow().getWidth();
        const auto height = (float)eg::Application::getInstance().getWindow().getHeight();
        m_cam = std::make_unique<eg::OrthographicCamera>((float)-width / 2, (float)width / 2, (float)-height / 2, (float)height / 2);

        srand(time(nullptr));
    }

    ~TheLayer()
    {
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
        }

        if (e.getEventType() == eg::EventType::MouseButtonPressed)
        {
            const auto mousecode = static_cast<eg::MouseButtonPressedEvent&>(e).getMouseButton();
            if (mousecode == eg::Mouse::ButtonLeft)
            {
                const auto pos = eg::Input::getMousePosition();
                const auto p = ScreenToGL({ pos.first, pos.second });
                
                eg::Rectangle2D rect{ rect_width, rect_height };
                eg::Color color{};
                color.r = rand() % 255;
                color.g = rand() % 255;
                color.b = rand() % 255;
                rect.setColor(color);
                rects.push_back(std::move(rect));

                eg::RigidBody body{};
                body.centroid_pos = p;
                bodies.push_back(body);
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
        
        for (int i = 0; i < rects.size(); i++)
        {
            for (int j = i + 1; j < rects.size(); j++)
            {
                if (const auto col_data = eg::physics::isCollide(&rects[i], &rects[j]); col_data.depth != 0.0f)
                {
                    const auto sig_face = eg::physics::findSignificantFace(*col_data.receptor, col_data.normal);

                    // const auto pp = eg::physics::findPentratePoint(col_data, sig_face);

                    eg::physics::resolveCollision(bodies[i], bodies[j], (&rects[i] == col_data.receptor) ? col_data.normal : -col_data.normal, col_data.depth);
                }
            }
        }
        
        updateWorld();

        for (int i = 0; i < rects.size(); i++)
        {
            rects[i].setPosition(bodies[i].centroid_pos);
            rects[i].update();
            rects[i].draw();
        }

        static int fps = 1;
        static eg::Clock clock{};
        if (clock.getElapsedTime().asSeconds() >= 1.0)
        {
            EG_TRACE("{}", fps);
            fps = 0;
            clock.restart();
        }
        fps++;
    }


private:
    void proccessInput()
    {
        constexpr float speed = 3.0f;

        if (eg::Input::isKeyPressed(eg::Key::W))
        {
            // triangle->translate({ 0.0f, speed });
        }
        if (eg::Input::isKeyPressed(eg::Key::S))
        {
            // triangle->translate({ 0.0f, -speed });
        }
        if (eg::Input::isKeyPressed(eg::Key::A))
        {
            // triangle->translate({ -speed, 0.0f });
        }
        if (eg::Input::isKeyPressed(eg::Key::D))
        {
            // triangle->translate({ speed, 0.0f });
        }
        if (eg::Input::isKeyPressed(eg::Key::E))
        {
            // triangle->rotate(-1.0f);
        }
        if (eg::Input::isKeyPressed(eg::Key::Q))
        {
            // triangle->rotate(1.0f);
        }
    }

    void updateWorld()
    {
        constexpr int step = 10;
        constexpr auto dt = 1.0f / 170.0f;
        constexpr auto dtt = dt / float(step);
        for (int i = 0; i < step; i++)
        {
            for (auto& body : bodies)
            {
                body.external_forces = { 0, -2000 }; // apply gravity

                body.updateVelocity(dtt);

                applyConstraint(body, dtt);

                body.updatePosition(dtt);
            }
        }
    }

    void applyConstraint(eg::RigidBody& body, const float dt) const
    {
        /*
        if (body.centroid_pos.y - 50.f >= -600.f)
            return;
        const auto beta = -(body.centroid_pos.y - 50.f + 600.f) * 0.4f / dt;
        const eg::Vector2f fc{ 0.0f, (beta - body.linear_velocity.y) * body.mass / dt };
        body.external_forces += fc;
        */
        eg::FloorConstraint constraint{};
        constraint.floor_h = -600.f;
        constraint.obj_half_height = rect_height / 2.0f;
        constraint.applyConstraint(body, dt);
    }

    static constexpr float rect_width = 100.f, rect_height = 100.f;

    std::unique_ptr<eg::OrthographicCamera> m_cam;

    std::vector<eg::RigidBody> bodies;
    std::vector<eg::Rectangle2D> rects;
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
