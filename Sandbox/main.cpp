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
                eg::Application::getInstance().onEvent(std::make_unique<eg::WindowCloseEvent>());
            }
        }
    }

    void onUpdate() override
    {
        degree += 0.00003f;
        if (degree >= 360.0f)
            degree -= 360.0f;
    }

    void onDraw() override
    {
        eg::Renderer2D::begin(cam);
        eg::Renderer2D::drawQuad(200, 150, 400, 300, eg::Color::Cyan, degree);
        eg::Renderer2D::end();
    }

private:
    float degree = 0.0f;
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