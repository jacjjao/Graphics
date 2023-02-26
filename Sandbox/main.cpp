#include "Graphics.hpp"
#include "include/Input/Input.hpp"
#include "include/Physics/RigidBody.hpp"
#include "include/Physics/World.hpp"
#include "include/Physics/Constraint.hpp"
#include "include/Physics/Utility.hpp"
#include "include/Physics/Collision.hpp"
#include "include/Core/RandomGenerator.hpp"


eg::Vector2f ScreenToGL(const eg::Vector2f pos)
{
    static const auto half_width = float(eg::Application::getInstance().getWindow().getWidth() / 2);
    static const auto half_height = float(eg::Application::getInstance().getWindow().getHeight() / 2);
    return {
        pos.x - half_width,
        half_height - pos.y
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
        
        rects.emplace_back(1900.f, 20.f);
        bodies.emplace_back();
        bodies.back().is_static = true;
        bodies.back().position = { 0.f, -500.f };
        bodies.back().width = 1900.f;
        bodies.back().height = 20.f;
        bodies.back().calcInertia();

        rects.emplace_back(800.f, 20.f);
        bodies.emplace_back();
        bodies.back().is_static = true;
        bodies.back().position = { -500.f, 0.f };
        bodies.back().rotate_radians = eg::radians(-20.f);
        bodies.back().width = 800.f;
        bodies.back().height = 20.f;
        bodies.back().calcInertia();

        rects.emplace_back(800.f, 20.f);
        bodies.emplace_back();
        bodies.back().is_static = true;
        bodies.back().position = { 500.f, 0.f };
        bodies.back().rotate_radians = eg::radians(20.f);
        bodies.back().width = 800.f;
        bodies.back().height = 20.f;
        bodies.back().calcInertia();

        rects.emplace_back(20.f, 1200.f);
        bodies.emplace_back();
        bodies.back().is_static = true;
        bodies.back().position = { 900.f, 0.f };
        bodies.back().width = 20.f;
        bodies.back().height = 600.f;
        bodies.back().calcInertia();

        rects.emplace_back(20.f, 1200.f);
        bodies.emplace_back();
        bodies.back().is_static = true;
        bodies.back().position = { -900.f, 0.f };
        bodies.back().width = 20.f;
        bodies.back().height = 600.f;
        bodies.back().calcInertia();
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
                color.r = rd.generate(0.f, 255.f);
                color.g = rd.generate(0.f, 255.f);
                color.b = rd.generate(0.f, 255.f);
                rect.setColor(color);
                rects.push_back(std::move(rect));

                eg::physics::RigidBody body{};
                body.type = eg::physics::RigidBodyType::Box;
                body.position = p;
                body.setMass(1000.f);
                body.width = body.height = 100.0f;
                body.calcInertia();
                bodies.push_back(body);
            }
        }
    }

    void onUpdate() override
    {
        static auto& program = eg::DefaultShaderProgram::instance();
        program.use();
        program.setMat4("view", m_cam->getViewMatrix());
        program.setMat4("proj", m_cam->getProjMatrix());
     
        updateWorld();

        for (int i = 0; i < rects.size(); i++)
        {
            rects[i].setPosition(bodies[i].position);
            rects[i].setRotation(bodies[i].rotate_radians * 57.295779513f);
            rects[i].update();
            rects[i].draw();
        }

        eg::Renderer2D::begin(*m_cam);
        for (const auto& v : vecs)
        {
            eg::Renderer2D::drawQuad(v, { 10, 10 }, eg::Color::Red);
        }
        eg::Renderer2D::end();

        static int fps = 1;
        static eg::Clock clock{};
        if (clock.getElapsedTime().asSeconds() >= 1.0)
        {
            // EG_TRACE("{}", fps);
            EG_TRACE("({},{}) {}", bodies.back().linear_velocity.x, bodies.back().linear_velocity.y, bodies.back().angular_velocity);
            fps = 0;
            clock.restart();
        }
        fps++;

        proccessInput();
    }


private:
    void proccessInput()
    {
        constexpr float speed = 3.0f;

        if (eg::Input::isKeyPressed(eg::Key::W))
        {
            bodies[1].linear_velocity += { 0.0f, speed };
        }
        if (eg::Input::isKeyPressed(eg::Key::S))
        {
            bodies[1].linear_velocity += { 0.0f, -speed };
        }
        if (eg::Input::isKeyPressed(eg::Key::A))
        {
            bodies[1].linear_velocity += { -speed, 0.0f };
        }
        if (eg::Input::isKeyPressed(eg::Key::D))
        {
            bodies[1].linear_velocity += { speed, 0.0f };
        }
        if (eg::Input::isKeyPressed(eg::Key::E))
        {
            bodies[1].rotate_radians -= 0.01f;
        }
        if (eg::Input::isKeyPressed(eg::Key::Q))
        {
            bodies[1].rotate_radians += 0.01f;
        }
        if (eg::Input::isKeyPressed(eg::Key::Space))
        {
            if (bodies.size() > 5)
            {
                bodies.resize(5);
                rects.resize(5);
            }
        }
    }

    void precompute()
    {
        polys.resize(rects.size());
        for (size_t i = 0; i < rects.size(); i++)
        {
            rects[i].setPosition(bodies[i].position);
            rects[i].setRotation(bodies[i].rotate_radians * 57.295779513f);

            polys[i].poly = &rects[i];
            polys[i].vertices.clear();
            polys[i].vertices.reserve(rects[i].getPointCount());            
            for (size_t j = 0; j < rects[i].getPointCount(); j++)
            {
                polys[i].vertices.push_back(rects[i].getPoint(j));
            }
        }
    }

    void updateWorld()
    {
        constexpr int step = 8;
        constexpr auto dt = 1.0f / 170.0f;
        constexpr auto dtt = dt / float(step);

        vecs.clear();

        for (int i = 0; i < step; i++)
        {
            // apply gravity
            for (auto& body : bodies)
            {
                constexpr eg::Vector2f gravity = { 0, -500.f };
                body.external_forces += gravity * body.getMass();
                body.update(dtt);
            }

            precompute();
            for (int i = 0; i < rects.size(); i++)
            {
                for (int j = i + 1; j < rects.size(); j++)
                {
                    if (bodies[i].is_static and bodies[j].is_static)
                    {
                        continue;
                    }
                    if (const auto result = eg::physics::isCollide(&polys[i], &polys[j]); result.has_value() and result.value().depth > 0.005f)
                    {
                        const auto& col_data = result.value();
                        const auto inc_face = eg::physics::findSignificantFace(col_data.receptor->vertices, col_data.normal);
                        const auto ref_face = eg::physics::findSignificantFace(col_data.donor->vertices, -col_data.normal);
                        const auto pp = eg::physics::findPentratePoint(inc_face, ref_face);
                        for (size_t i = 0; i < pp.cnt; i++)
                        {
                            vecs.push_back(pp.cp[i]);
                        }

                        auto& receptor = (col_data.receptor == &polys[i]) ? bodies[i] : bodies[j];
                        auto& donor = (col_data.donor == &polys[i]) ? bodies[i] : bodies[j];
                        eg::physics::resolveCollision(receptor, donor, pp, col_data.normal, col_data.depth);
                    }
                }
            }

            for (auto& body : bodies)
            {
                body.update(dtt);
            }
        }                   
    }

    void applyConstraint(eg::physics::RigidBody& body, const float dt) const
    {
        /*
        eg::physics::FloorConstraint constraint{};
        constraint.floor_h = -600.f;
        constraint.obj_half_height = rect_height / 2.0f;
        constraint.applyConstraint(body, dt);

        if (body.position.x - 50.f < -1000.0f)
        {
            body.position.x = -1000.0f + 50.f;
            body.linear_velocity.x *= -1.0f;
        }
        else if (body.position.x + 50.f > 1000.f)
        {
            body.position.x = 1000.0f - 50.f;
            body.linear_velocity.x *= -1.0f;
        }
        */
    }

    static constexpr float rect_width = 100.f, rect_height = 100.f;

    std::unique_ptr<eg::OrthographicCamera> m_cam;

    std::vector<eg::physics::RigidBody> bodies;
    std::vector<eg::Rectangle2D> rects;
    std::vector<eg::physics::Polygon> polys;

    std::vector<eg::Vector2f> vecs;

    eg::RandomFloatGenerator rd;
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
