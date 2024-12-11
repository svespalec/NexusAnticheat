#pragma once
#include <cstdint>
#include <string_view>

namespace nexus::hash
{
constexpr uint64_t BASIS = 14695981039346656037ULL;
constexpr uint64_t PRIME = 1099511628211ULL;

constexpr uint64_t fnv1a( const char* str, const uint64_t value = BASIS ) noexcept
{
	return ( str[0] == '\0' ) ? value : fnv1a( &str[1], ( value ^ uint64_t( str[0] ) ) * PRIME );
}

constexpr uint64_t fnv1a( const std::string_view& str ) noexcept
{
	uint64_t hash = BASIS;

	for ( auto c : str )
	{
		hash ^= c;
		hash *= PRIME;
	}

	return hash;
}

namespace feature_hashes
{
constexpr uint64_t memory_check    = fnv1a( "memory_check" );
constexpr uint64_t process_check   = fnv1a( "process_check" );
constexpr uint64_t integrity_check = fnv1a( "integrity_check" );
constexpr uint64_t hook_check      = fnv1a( "hook_check" );
// Add more feature hashes here
}
} // namespace nexus::hash

#define FNV64( str ) nexus::hash::fnv1a( str )
