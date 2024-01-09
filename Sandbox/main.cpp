#include "Graphics.hpp"
#include "include/Core/FileSystem.hpp"

class TheLayer : public eg::Layer
{
public:
    TheLayer() : 
        cam(0, 800, 600, 0)
    {
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
        eg::Renderer2D::begin(cam);
        eg::Renderer2D::drawQuad(400, 300, 400, 300, eg::Color::Red);
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