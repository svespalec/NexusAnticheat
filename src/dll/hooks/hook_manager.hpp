#pragma once
#include <Windows.h>
#include <MinHook.h>
#include <utils/logger.hpp>

namespace nexus::hooks
{
class hook_manager
{
public:
	static bool initialize()
	{
		if ( const MH_STATUS status = MH_Initialize(); status != MH_OK )
		{
			log_error( "Failed to initialize MinHook: {}", MH_StatusToString( status ) );
			return false;
		}

		return true;
	}

	static void shutdown()
	{
		MH_DisableHook( MH_ALL_HOOKS );
		MH_Uninitialize();
	}

	template <typename T>
	static bool create_hook( LPVOID target, LPVOID detour, T** original )
	{
		if ( const MH_STATUS status = MH_CreateHook( target, detour, reinterpret_cast<LPVOID*>( original ) );
		     status != MH_OK )
		{
			log_error( "Failed to create hook: {}", MH_StatusToString( status ) );
			return false;
		}

		if ( const MH_STATUS status = MH_EnableHook( target ); status != MH_OK )
		{
			log_error( "Failed to enable hook: {}", MH_StatusToString( status ) );
			return false;
		}

		return true;
	}
};
}
