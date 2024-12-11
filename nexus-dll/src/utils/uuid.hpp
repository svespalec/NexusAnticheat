#pragma once

#include <string>
#include <random>

namespace nexus::utils
{
class uuid
{
public:
	static std::string generate()
	{
		static std::random_device              rd;
		static std::mt19937_64                 gen( rd() );
		static std::uniform_int_distribution<> dis( 0, 15 );
		static std::uniform_int_distribution<> dis2( 8, 11 );

		std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";

		for ( char& c : uuid )
		{
			if ( c == 'x' )
				c = get_hex_char( static_cast<char>( dis( gen ) ) );
			else if ( c == 'y' )
				c = get_hex_char( static_cast<char>( dis2( gen ) ) );
		}

		return uuid;
	}

private:
	static char get_hex_char( char v )
	{
		if ( v < 10 )
			return static_cast<char>( '0' + v );

		return static_cast<char>( 'a' + ( v - 10 ) );
	}
};

} // namespace nexus::utils
