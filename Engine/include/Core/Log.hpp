#pragma once

#include "include/Core/Core.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>

namespace eg
{
	class Log
	{
	private:
		using logger_ptr = std::shared_ptr<spdlog::logger>;
	public:
		static void Init();

		static logger_ptr& GetCoreLogger() { return s_CoreLogger; }
		static logger_ptr& GetClientLogger() { return s_ClientLogger; }

	private:
		static logger_ptr s_CoreLogger;
		static logger_ptr s_ClientLogger;
	};
} // namespace eg

// Core log macros
#define EG_CORE_TRACE(...) ::eg::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EG_CORE_INFO(...)  ::eg::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EG_CORE_WARN(...)  ::eg::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EG_CORE_ERROR(...) ::eg::Log::GetCoreLogger()->error(__VA_ARGS__)

// Client log macros
#define EG_TRACE(...) ::eg::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EG_INFO(...)  ::eg::Log::GetClientLogger()->info(__VA_ARGS__)
#define EG_WARN(...)  ::eg::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EG_ERROR(...) ::eg::Log::GetClientLogger()->error(__VA_ARGS__)