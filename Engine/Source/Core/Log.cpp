#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace AN
{
	std::shared_ptr<spdlog::logger> FLogEngine::Logger = nullptr;

	void FLogEngine::Init(unsigned int LogLevel)
	{
		spdlog::level::level_enum Level = static_cast<spdlog::level::level_enum>(LogLevel);

		std::vector<spdlog::sink_ptr> Sinks;
		Sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		Sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Log.txt", true));

		Logger = std::make_shared<spdlog::logger>("AN", begin(Sinks), end(Sinks));
		spdlog::register_logger(Logger);
		Logger->set_level(Level);
		Logger->flush_on(Level);

		LOG_TRACE("Logger initialized")
	}
}

