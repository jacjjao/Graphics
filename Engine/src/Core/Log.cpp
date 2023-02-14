#include "pch.hpp"
#include "include/Core/Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace eg
{
	Log::logger_ptr Log::s_CoreLogger = nullptr;
	Log::logger_ptr Log::s_ClientLogger = nullptr;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("eg");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}