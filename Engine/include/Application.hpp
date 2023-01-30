#pragma once

class Application
{
public:
	virtual ~Application() = default;

	Application& instance();

	void run();

private:
	explicit Application();
	void onWindowClosed();

	static Application* s_instanced;

	bool m_running = true;
};