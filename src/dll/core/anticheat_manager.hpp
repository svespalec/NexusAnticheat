#pragma once

#include "feature_registry.hpp"
#include "../utils/hash.hpp"
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <unordered_map>

namespace nexus
{
class anticheat_manager
{
public:
	static anticheat_manager& get_instance()
	{
		static anticheat_manager instance;
		return instance;
	}

	anticheat_manager( const anticheat_manager& )            = delete;
	anticheat_manager& operator=( const anticheat_manager& ) = delete;
	anticheat_manager( anticheat_manager&& )                 = delete;
	anticheat_manager& operator=( anticheat_manager&& )      = delete;

	bool initialize()
	{
		if ( initialized_ )
			return true;

		should_run_  = true;
		scan_thread_ = std::make_unique<std::thread>( &anticheat_manager::scan_routine, this );
		initialized_ = true;

		return true;
	}

	void shutdown()
	{
		if ( !initialized_ )
			return;

		should_run_ = false;

		if ( scan_thread_ && scan_thread_->joinable() )
			scan_thread_->join();

		for ( auto& [_, feature] : features_ )
		{
			if ( feature )
				feature->shutdown();
		}

		features_.clear();
		initialized_ = false;
	}

	template <typename T>
	bool register_feature()
	{
		static_assert( std::is_base_of_v<feature_base, T>, "T must derive from feature_base" );
		feature_registry::register_feature<T>();

		return true;
	}

	bool enable_feature( uint64_t feature_hash )
	{
		std::lock_guard<std::mutex> lock( mutex_ );

		auto it = features_.find( feature_hash );

		if ( it == features_.end() )
		{
			auto feature = feature_registry::create_feature( feature_hash );

			if ( !feature )
				return false;

			if ( !feature->initialize() )
				return false;

			feature->set_enabled( true );
			features_[feature_hash] = std::move( feature );

			return true;
		}

		it->second->set_enabled( true );

		return true;
	}

	void disable_feature( uint64_t feature_hash )
	{
		std::lock_guard<std::mutex> lock( mutex_ );

		auto it = features_.find( feature_hash );

		if ( it != features_.end() )
		{
			it->second->set_enabled( false );
		}
	}

	void set_scan_interval( std::chrono::milliseconds interval )
	{
		scan_interval_ = interval;
	}

	bool is_initialized() const
	{
		return initialized_;
	}

private:
	anticheat_manager() = default;

	~anticheat_manager()
	{
		shutdown();
	}

	std::atomic<bool>         initialized_{ false };
	std::atomic<bool>         should_run_{ false };
	std::chrono::milliseconds scan_interval_{ 1000 };

	std::mutex                                                  mutex_;
	std::unordered_map<uint64_t, std::unique_ptr<feature_base>> features_;
	std::unique_ptr<std::thread>                                scan_thread_;

	void check_features()
	{
		std::lock_guard<std::mutex> lock( mutex_ );

		for ( auto& [_, feature] : features_ )
		{
			if ( feature && feature->is_enabled() )
				feature->run_check();
		}
	}

	void scan_routine()
	{
		while ( should_run_ )
		{
			check_features();
			std::this_thread::sleep_for( scan_interval_ );
		}
	}
};

} // namespace nexus
