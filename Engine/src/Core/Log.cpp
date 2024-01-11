#include "pch.hpp"
#include "include/Core/Log.hpp"

namespace eg
{
	Log::logger_ptr Log::s_CoreLogger = [] 
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		auto core_logger = spdlog::stdout_color_mt("eg");
		core_logger->set_level(spdlog::level::trace);
		return core_logger;
	}();

	Log::logger_ptr Log::s_ClientLogger = []
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        auto client_logger = spdlog::stdout_color_mt("APP");
        client_logger->set_level(spdlog::level::trace);
        return client_logger;
    }();
}