#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <filesystem>
#include "logger.hpp"
#include "uuid.hpp"

namespace nexus::report
{
class report_manager
{
public:
	static report_manager& get_instance()
	{
		static report_manager instance;
		return instance;
	}

	void add_flag( const std::string& flag_name )
	{
		std::lock_guard<std::mutex> lock( mutex_ );
		flags_[flag_name]++;

		if ( !file_created_ )
			create_report_file();

		update_report_file();
	}

	void shutdown()
	{
		std::lock_guard<std::mutex> lock( mutex_ );

		if ( report_file_.is_open() )
			report_file_.close();
	}

private:
	report_manager() = default;

	~report_manager()
	{
		shutdown();
	}

	std::filesystem::path get_flags_directory()
	{
		char home_path[MAX_PATH];

		if ( GetEnvironmentVariableA( "USERPROFILE", home_path, MAX_PATH ) == 0 )
		{
			log_error( "failed to get user profile path" );
			return {};
		}

		auto flags_dir = std::filesystem::path( home_path ) / "Desktop" / "NexusAnticheat" / "Flags";

		if ( !std::filesystem::exists( flags_dir ) )
		{
			std::error_code ec{};

			if ( !std::filesystem::create_directories( flags_dir, ec ) )
			{
				log_error( "failed to create flags directory: {}", ec.message() );
				return {};
			}
		}

		return flags_dir;
	}

	void create_report_file()
	{
		auto flags_dir = get_flags_directory();

		if ( flags_dir.empty() )
			return;

		report_path_ = flags_dir / std::format( "flags-{}.txt", utils::uuid::generate() );
		report_file_.open( report_path_, std::ios::out | std::ios::trunc );

		if ( !report_file_.is_open() )
		{
			log_error( "failed to create report file: {}", report_path_.string() );
			return;
		}

		file_created_ = true;

		log_info( "created new report file: {}", report_path_.string() );
	}

	void update_report_file()
	{
		if ( !report_file_.is_open() )
			return;

		report_file_.seekp( 0 );
		report_file_.clear();

		for ( const auto& [flag, count] : flags_ )
			report_file_ << std::format( "{} ({})\n", flag, count );

		report_file_.flush();
	}

private:
	std::mutex                           mutex_;
	std::unordered_map<std::string, int> flags_;
	std::ofstream                        report_file_;
	std::filesystem::path                report_path_;
	bool                                 file_created_ = false;
};

} // namespace nexus::report
