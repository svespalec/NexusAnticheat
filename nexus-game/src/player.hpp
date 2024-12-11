#pragma once

namespace game
{
class player
{
public:
	player();

	float get_health() const;
	void  set_health( float health );
	void  damage( float amount );
	void  heal( float amount );

private:
	float health_;
};
} // namespace game
