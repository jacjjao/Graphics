#pragma once

#include "include/Core/Core.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>

namespace Engine
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
} // namespace Engine

// Core log macros
#define EG_CORE_TRACE(...) ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EG_CORE_INFO(...)  ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EG_CORE_WARN(...)  ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EG_CORE_ERROR(...) ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)

// Client log macros
#define EG_TRACE(...) ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EG_INFO(...)  ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define EG_WARN(...)  ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EG_ERROR(...) ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)