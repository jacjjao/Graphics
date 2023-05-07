#include "Graphics.hpp"
#include "include/Core/FileSystem.hpp"

class TheLayer : public eg::Layer
{
public:
    TheLayer()
    {
        tex = std::make_shared<eg::Texture>(FileSystem::getPath("/asset/images/container.jpg"));

        m_rect = new eg::Rectangle2D{ 500.f, 500.f };
        m_rect->setColor(eg::Color::Blue);
        m_rect->applyTexture(tex);
        eg::Rect r = m_rect->getTextureRect();
        r.size.x /= 2.0f;
        r.size.y /= 2.0f;
        m_rect->setTextureRect(r);
        m_rect->update();
    }

    ~TheLayer() override
    {
        delete m_rect;
    }

    void onAttach() override {}
    void onDetach() override {}
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
        m_rect->draw();
    }

private:
    eg::Rectangle2D* m_rect;
    std::shared_ptr<eg::Texture> tex;
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