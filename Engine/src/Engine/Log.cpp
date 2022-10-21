#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;


	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
		s_ClientLogger = spdlog::stdout_color_mt("APP");

	}
}

