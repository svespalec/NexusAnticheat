#include "player.hpp"
#include <algorithm>

namespace game
{
player::player()
    : health_( 100.0f )
{
}

float player::get_health() const
{
	return health_;
}

void player::set_health( float health )
{
	health_ = std::clamp( health, 0.0f, 100.0f );
}

void player::damage( float amount )
{
	health_ = std::clamp( health_ - amount, 0.0f, 100.0f );
}

void player::heal( float amount )
{
	health_ = std::clamp( health_ + amount, 0.0f, 100.0f );
}

} // namespace game
