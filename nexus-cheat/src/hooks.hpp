#pragma once
#include <Windows.h>
#include <MinHook.h>

namespace cheat::hooks
{
using get_health_t = float( __fastcall* )();
using damage_t     = void( __fastcall* )( float );

extern get_health_t original_get_health;
extern damage_t     original_damage;

float __fastcall hooked_get_health();
void __fastcall hooked_damage( float amount );

bool initialize();
void shutdown();
}
