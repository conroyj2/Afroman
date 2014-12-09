#include "Player.h"

Player::Player(void)
{
	//player's initial settings

	state = Player::State::Standing;
	facingRight = true;
	grounded = true;
	falling = false;
	climbing = false;
	velocity = Point(0, 0);
	position = Point();
}

Player::~Player(void)
{
}