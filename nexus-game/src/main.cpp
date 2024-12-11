#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "player.hpp"

int main()
{
	game::player player{};

	// simulate game loop
	while ( true )
	{
		std::cout << "player health: " << player.get_health() << std::endl;

		// take some damage every few seconds to simulate gameplay
		player.damage( 5.0f );

		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
	}

	return 0;
}
