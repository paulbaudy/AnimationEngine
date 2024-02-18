#pragma once

#include "spdlog/spdlog.h"

#include "Core/Core.h"

namespace AN
{
	class FLogEngine
	{
	public:
		static void Init(unsigned int LogLevel);

		static std::shared_ptr<spdlog::logger> Logger;
	};
}

#define LOG_INFO(...) AN::FLogEngine::Logger->info(__VA_ARGS__);
#define LOG_TRACE(...) AN::FLogEngine::Logger->trace(__VA_ARGS__);
