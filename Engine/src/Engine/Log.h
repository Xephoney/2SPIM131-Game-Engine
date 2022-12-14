#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Engine
{
	class  Log
	{
	public:
		static void Initialize();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()		{ return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()	{ return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core logging macros :)
#define ENGINE_LOG_ERROR(...)      ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__);
#define ENGINE_LOG_WARNING(...)    ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define ENGINE_LOG_INFO(...)       ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__);
#define ENGINE_LOG_TRACE(...)      ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define ENGINE_LOG_FATAL(...)      ::Engine::Log::GetCoreLogger()->fatal(__VA_ARGS__);

// Client logging macros :)
#define LOG_ERROR(...)      ::Engine::Log::GetClientLogger()->error(__VA_ARGS__); 
#define LOG_WARNING(...)    ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__);
#define LOG_INFO(...)       ::Engine::Log::GetClientLogger()->info(__VA_ARGS__); 
#define LOG_TRACE(...)      ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__);
#define LOG_FATAL(...)      ::Engine::Log::GetClientLogger()->fatal(__VA_ARGS__);

