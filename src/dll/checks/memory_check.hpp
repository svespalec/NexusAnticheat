#pragma once

#include <Windows.h>
#include <memory>
#include <vector>
#include <utils/logger.hpp>
#include <utils/report_manager.hpp>
#include "../core/feature_registry.hpp"

namespace nexus::checks
{
class memory_check final : public feature_base
{
public:
	static constexpr uint64_t feature_hash = hash::feature_hashes::memory_check;

	memory_check()           = default;
	~memory_check() override = default;

	bool initialize() override
	{
		log_info( "initializing memory check" );

		return true;
	}

	void shutdown() override
	{
		log_info( "shutting down memory check" );
	}

	bool run_check() override
	{
		scan_protected_regions();
		check_memory_integrity();

		return true;
	}

	uint64_t get_feature_hash() const override
	{
		return feature_hash;
	}

private:
	void scan_protected_regions()
	{
		log_info( "scan_protected_regions" );

		// example flag for testing
		report::report_manager::get_instance().add_flag( ".text section patched" );
	}

	void check_memory_integrity()
	{
		log_info( "check_memory_integrity" );

		// example flag for testing
		report::report_manager::get_instance().add_flag( "memory integrity violation" );
	}
};

} // namespace nexus::checks
