#include "core/anticheat_manager.hpp"
#include "checks/memory_check.hpp"
#include <Windows.h>
#include "utils/logger.hpp"
#include "utils/report_manager.hpp"
#include "checks/tls_callback.hpp"
#include "hooks/hook_manager.hpp"
#include "hooks/messagebox_hook.hpp"

bool setup_anticheat()
{
	if ( !nexus::log::logger::initialize() )
		return false;

	log_info( "initializing anticheat..." );

	if ( !nexus::hooks::hook_manager::initialize() )
	{
		log_error( "Failed to initialize hook manager!" );
		return false;
	}

	if ( !nexus::hooks::messagebox_hook::initialize() )
	{
		log_error( "Failed to initialize MessageBoxA hook!" );
		return false;
	}

	MessageBoxA( NULL, "Nexus Anticheat", "Nexus Anticheat", MB_OK );

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
		nexus::hooks::hook_manager::shutdown();
		nexus::anticheat_manager::get_instance().shutdown();
		nexus::report::report_manager::get_instance().shutdown();
		nexus::log::logger::shutdown();
		break;
	}

	return TRUE;
}
