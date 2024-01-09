#include "Graphics.hpp"
#include "include/Core/FileSystem.hpp"

class TheLayer : public eg::Layer
{
public:
    TheLayer() : 
        cam(800, 600)
    {
        //cam.move({-400, -300, 0});
    }

    ~TheLayer() override = default;

    void onAttach() override
    {
        eg::Renderer2D::Init();
    }

    void onDetach() override 
    {
    }

    void onEvent(eg::Event& e) override
    {
        if (e.getEventType() == eg::EventType::KeyPressed)
        {
            const int keycode = static_cast<eg::KeyPressedEvent&>(e).GetKeyCode();
            if (keycode == eg::Key::Escape)
            {
                eg::WindowCloseEvent close_signal{};
                eg::Application::getInstance().onEvent(close_signal);
            }
        }
    }

    void onUpdate() override
    {
    }

    void onDraw() override
    {
        static float degree = 90.0f;
        degree += 1.0f;
        if (degree >= 360.0f)
            degree -= 360.0f;
        eg::Renderer2D::begin(cam);
        eg::Renderer2D::drawQuad(200, 150, 400, 300, eg::Color::Red, degree);
        eg::Renderer2D::end();
    }

private:
    eg::OrthographicCamera cam;
    std::shared_ptr<eg::Texture> tex;
};

int main()
{
    eg::Log::Init();

    eg::Application app(800, 600, "");
    app.setTargetFps(60);
    app.setClearColor(eg::Color::Grey);
    app.pushLayer(new TheLayer{});
    app.run();

    return 0;
}