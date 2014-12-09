#ifndef  _PLAYER_H_
#define  _PLAYER_H_

#include "cocos2d.h"
USING_NS_CC;

class Player : public Object
{

public:
	enum State {
		Standing, Walking, Jumping, Climbing
	};

	State state;

	bool facingRight;
	bool grounded;
	bool falling;
	bool climbing;

	Size player_size;

	Point position;
	Point velocity;

	Player(void);
	virtual ~Player(void);
};

#endif // _PLAYER_H_