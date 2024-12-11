#include "hooks.hpp"
#include <iostream>

namespace cheat::hooks
{
get_health_t original_get_health = nullptr;
damage_t     original_damage     = nullptr;

float __fastcall hooked_get_health()
{
	// always return full health
	return 100.0f;
}

void __fastcall hooked_damage( float amount )
{
	// ignore all damage
	std::cout << "blocked damage amount: " << amount << std::endl;
}

bool initialize()
{
	if ( MH_Initialize() != MH_OK )
		return false;

	// get the base address of the game module
	HMODULE game_module = GetModuleHandleA( nullptr );

	if ( !game_module )
		return false;

	// find the get_health and damage functions
	// TODO: use pattern scanning or other methods
	void* get_health_addr = nullptr;
	void* damage_addr     = nullptr;

	if ( MH_CreateHook( get_health_addr, &hooked_get_health, reinterpret_cast<LPVOID*>( &original_get_health ) ) != MH_OK )
		return false;

	if ( MH_CreateHook( damage_addr, &hooked_damage, reinterpret_cast<LPVOID*>( &original_damage ) ) != MH_OK )
		return false;

	if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
		return false;

	return true;
}

void shutdown()
{
	MH_DisableHook( MH_ALL_HOOKS );
	MH_Uninitialize();
}
}
