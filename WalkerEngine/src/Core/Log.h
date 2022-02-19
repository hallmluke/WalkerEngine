#pragma once
#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/wincolor_sink.h"

namespace Walker {
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger;  }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define W_CORE_ERROR(...) ::Walker::Log::GetCoreLogger()->error(__VA_ARGS__)
#define W_CORE_INFO(...)  ::Walker::Log::GetCoreLogger()->info(__VA_ARGS__)
#define W_CORE_WARN(...)  ::Walker::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define W_CORE_TRACE(...) ::Walker::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define W_CORE_FATAL(...) ::Walker::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define W_ERROR(...) ::Walker::Log::GetClientLogger()->error(__VA_ARGS__)
#define W_INFO(...)  ::Walker::Log::GetClientLogger()->info(__VA_ARGS__)
#define W_WARN(...)  ::Walker::Log::GetClientLogger()->warn(__VA_ARGS__)
#define W_TRACE(...) ::Walker::Log::GetClientLogger()->trace(__VA_ARGS__)
#define W_FATAL(...) ::Walker::Log::GetClientLogger()->fatal(__VA_ARGS__)