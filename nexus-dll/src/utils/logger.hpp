#pragma once

#include <Windows.h>
#include <iostream>
#include <mutex>
#include <format>
#include <chrono>

#define ENABLE_LOGGING

namespace nexus::log
{
enum class level
{
	info,
	warning,
	error,
	success,
	detection
};

static std::atomic<bool> initialized{};

class logger
{
public:
	static bool initialize()
	{
		if ( initialized )
			return true;


		if ( !AllocConsole() )
			return false;

		FILE* p_file = nullptr;

		if ( freopen_s( &p_file, "CONOUT$", "w", stdout ) != 0 )
			return false;

		if ( freopen_s( &p_file, "CONIN$", "r", stdin ) != 0 )
			return false;

		HANDLE output = GetStdHandle( STD_OUTPUT_HANDLE );
		DWORD  mode{ 0 };

		if ( GetConsoleMode( output, &mode ) )
		{
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

			if ( !SetConsoleMode( output, mode ) )
				return false;
		}

		SetConsoleTitleA( "Nexus Anticheat [DEBUG]" );

		initialized = true;

		return true;
	}

	static void shutdown()
	{
		system( "cls" );

		fclose( stdin );
		fclose( stdout );
		fclose( stderr );

		FreeConsole();
	}

	template <typename... Args>
	static void print( level log_level, [[maybe_unused]] const char* func_name, const char* fmt, Args... args )
	{
		std::lock_guard<std::mutex> lock( get_mutex() );

		if ( !initialized )
			return;

		const auto timestamp = get_timestamp();
		const auto message   = std::vformat( fmt, std::make_format_args( args... ) );

		std::cout << "[\033[90m" << timestamp << "\033[0m] [" << get_level_color( log_level )
		          << get_level_string( log_level ) << "\033[0m]";

#ifdef SHOW_FUNCTION_NAMES
		std::cout << " [\033[90m" << func_name << "\033[0m]";
#endif

		std::cout << ": " << message << "\n";
	}

private:
	static std::mutex& get_mutex()
	{
		static std::mutex mutex;
		return mutex;
	}

	static const char* get_level_color( level log_level )
	{
		switch ( log_level )
		{
		case level::info:
			return "\033[36m"; // Cyan
		case level::warning:
			return "\033[33m"; // Yellow
		case level::error:
			return "\033[31m"; // Red
		case level::success:
			return "\033[32m"; // Green
		case level::detection:
			return "\033[35m"; // Magenta
		default:
			return "\033[0m"; // Reset
		}
	}

	static std::string get_level_string( level log_level )
	{
		switch ( log_level )
		{
		case level::info:
			return "INFO";
		case level::warning:
			return "WARN";
		case level::error:
			return "ERROR";
		case level::success:
			return "SUCCESS";
		case level::detection:
			return "DETECT";
		default:
			return "UNKNOWN";
		}
	}

	static std::string get_timestamp()
	{
		auto    now   = std::chrono::system_clock::now();
		auto    timer = std::chrono::system_clock::to_time_t( now );
		std::tm bt;
		localtime_s( &bt, &timer );

		char buffer[32];
		strftime( buffer, sizeof( buffer ), "%I:%M:%S %p", &bt );

		std::string time_str = buffer;
		if ( time_str[0] == '0' )
			time_str = time_str.substr( 1 );

		return time_str;
	}
};

} // namespace nexus::log

// Define logging macros
#ifdef ENABLE_LOGGING
#define log_info( fmt, ... )      nexus::log::logger::print( nexus::log::level::info, __FUNCTION__, fmt, __VA_ARGS__ )
#define log_warning( fmt, ... )   nexus::log::logger::print( nexus::log::level::warning, __FUNCTION__, fmt, __VA_ARGS__ )
#define log_error( fmt, ... )     nexus::log::logger::print( nexus::log::level::error, __FUNCTION__, fmt, __VA_ARGS__ )
#define log_success( fmt, ... )   nexus::log::logger::print( nexus::log::level::success, __FUNCTION__, fmt, __VA_ARGS__ )
#define log_detection( fmt, ... ) nexus::log::logger::print( nexus::log::level::detection, __FUNCTION__, fmt, __VA_ARGS__ )
#else
#define log_info( fmt, ... )
#define log_warning( fmt, ... )
#define log_error( fmt, ... )
#define log_success( fmt, ... )
#define log_detection( fmt, ... )
#endif
