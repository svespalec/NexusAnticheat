#pragma once

#include <windows.h>
#include <utils/logger.hpp>

namespace nexus::checks
{
#pragma comment( linker, "/INCLUDE:_tls_used" )
#pragma comment( linker, "/INCLUDE:thread_callback" )
#pragma const_seg( ".CRT$XLB" )

#pragma optimize( "", off )
EXTERN_C const
    VOID WINAPI
    thread_callback( PVOID dll_handle, DWORD reason, PVOID reserved )
{
	if ( reason == DLL_PROCESS_ATTACH || reason == DLL_THREAD_ATTACH )
	{
		// TODO
	}
};
#pragma optimize( "", on )

#pragma const_seg()
#pragma comment( linker, "/SECTION:.CRT,R" )
} // namespace nexus::checks
