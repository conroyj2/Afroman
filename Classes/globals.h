#ifndef  _GLOBALS_H_
#define  _GLOBALS_H_

#include "cocos2d.h"
USING_NS_CC;
 
// keycodes

const EventKeyboard::KeyCode ENTER = EventKeyboard::KeyCode::KEY_KP_ENTER;
const EventKeyboard::KeyCode UP_ARROW = EventKeyboard::KeyCode::KEY_UP_ARROW;
const EventKeyboard::KeyCode DOWN_ARROW = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
const EventKeyboard::KeyCode LEFT_ARROW = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
const EventKeyboard::KeyCode RIGHT_ARROW = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
const EventKeyboard::KeyCode SPACEBAR = EventKeyboard::KeyCode::KEY_SPACE;
const EventKeyboard::KeyCode SNARE = EventKeyboard::KeyCode::KEY_F;
const EventKeyboard::KeyCode HI_HAT = EventKeyboard::KeyCode::KEY_J;
const EventKeyboard::KeyCode TOM1 = EventKeyboard::KeyCode::KEY_R;
const EventKeyboard::KeyCode TOM2 = EventKeyboard::KeyCode::KEY_T;
const EventKeyboard::KeyCode TOM3 = EventKeyboard::KeyCode::KEY_Y;
const EventKeyboard::KeyCode CYMBAL = EventKeyboard::KeyCode::KEY_U;
 
const float SCALE_FACTOR = 2.0f; //only used in gamescene 1
const float PLAYER_JUMP_VELOCITY = 30.0f;
const float GRAVITY = 2.0f;

#endif // _GLOBALS_H_