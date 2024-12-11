#include "core/anticheat_manager.hpp"
#include <Windows.h>

BOOL APIENTRY DllMain( HMODULE module, DWORD reason, [[maybe_unused]] LPVOID reserved )
{
	if ( reason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( module );
	}

	return TRUE;
}
