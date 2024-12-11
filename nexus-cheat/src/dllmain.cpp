#include <Windows.h>
#include "hooks.hpp"

BOOL APIENTRY DllMain( HMODULE module, DWORD reason, [[maybe_unused]] LPVOID reserved )
{
	switch ( reason )
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( module );

		if ( !cheat::hooks::initialize() )
			return false;

		break;

	case DLL_PROCESS_DETACH:
		cheat::hooks::shutdown();
		break;
	}

	return true;
}
