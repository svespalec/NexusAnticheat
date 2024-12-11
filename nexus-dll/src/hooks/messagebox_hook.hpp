#pragma once
#include <Windows.h>
#include "hook_manager.hpp"

namespace nexus::hooks
{
class messagebox_hook
{
public:
	static bool initialize()
	{
		return hook_manager::create_hook(
		    &MessageBoxA,
		    &hooked_messagebox,
		    &original_messagebox );
	}

private:
	static inline decltype( &MessageBoxA ) original_messagebox = nullptr;

	static int WINAPI hooked_messagebox(
	    _In_opt_ HWND   hwnd,
	    _In_opt_ LPCSTR lpText,
	    _In_opt_ LPCSTR lpCaption,
	    _In_ UINT       uType )
	{
		log_info( "MessageBoxA called with text: {}", lpText ? lpText : "null" );

		return original_messagebox( hwnd, lpText, lpCaption, uType );
	}
};
}
