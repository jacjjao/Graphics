#include "Graphics.hpp"
#include "include/Input/Input.hpp"

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
            switch (keycode)
            {
            case eg::Key::Escape:
                eg::Application::getInstance().onEvent(std::make_unique<eg::WindowCloseEvent>());
                break;
            }
        }
    }

    void onUpdate() override
    {
        const float step = 0.0001;
        if (eg::Input::isKeyPressed(eg::Key::W))
        {
            y -= step;
        }
        if (eg::Input::isKeyPressed(eg::Key::S))
        {
            y += step;
        }
        if (eg::Input::isKeyPressed(eg::Key::A))
        {
            x -= step;
        }
        if (eg::Input::isKeyPressed(eg::Key::D))
        {
            x += step;
        }
        if (eg::Input::isKeyPressed(eg::Key::E))
        {
            degree -= step;
        }
        if (eg::Input::isKeyPressed(eg::Key::Q))
        {
            degree += step;
        }
        if (degree >= 360.0)
            degree -= 360.0;
        else if (degree <= -360.0)
            degree += 360.0f;
    }

    void onDraw() override
    {
        eg::Renderer2D::begin(cam);
        eg::Renderer2D::drawQuad(x, y, 100, 100, eg::Color::Cyan, degree);
        eg::Renderer2D::end();
        eg::TextRenderer::renderText("Hello World", {100, 100});
    }

private:
    float degree = 0.0f;
    float x = 350, y = 250;
    eg::OrthographicCamera cam;
    std::shared_ptr<eg::Texture> tex;
};

int main()
{
    eg::Application app(800, 600, "");
    app.setTargetFps(170);
    app.setClearColor(eg::Color::Grey);
    app.pushLayer(new TheLayer{});
    app.run();

    return 0;
}