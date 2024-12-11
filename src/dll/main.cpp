#include "core/anticheat_manager.hpp"
#include "checks/memory_check.hpp"
#include <Windows.h>
#include "utils/logger.hpp"

bool setup_anticheat()
{
	if ( !nexus::log::logger::initialize() )
		return false;

	log_info( "initializing anticheat..." );

	auto& manager = nexus::anticheat_manager::get_instance();

	// register all features
	manager.register_feature<nexus::checks::memory_check>();

	if ( !manager.initialize() )
		return false;

	if ( !manager.enable_feature( nexus::hash::feature_hashes::memory_check ) )
		return false;
	
	// set custom scan interval (optional)
	manager.set_scan_interval( std::chrono::milliseconds( 2500 ) );

	log_success( "anticheat initialized!" );

	return true;
}

BOOL APIENTRY DllMain( HMODULE module, DWORD reason, [[maybe_unused]] LPVOID reserved )
{
	switch ( reason )
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( module );

		if ( !setup_anticheat() )
		{
			log_error( "failed to initialize anticheat!" );
			return FALSE;
		}

		break;
	case DLL_PROCESS_DETACH:
		// cleanup anticheat
		nexus::anticheat_manager::get_instance().shutdown();
		break;
	}

	return TRUE;
}
