#ifndef  _START_SCREEN_H_
#define  _START_SCREEN_H_

#include "cocos2d.h"
#include "Level.h"
#include "Player.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
USING_NS_CC;

class StartScreen : public cocos2d::Layer
{
public:

	//FIELDS
	Sprite *cameraTarget;
	Sprite *selectMenubg;

	Sprite *sprite;
	Sprite *sprite2;

	Follow *camera;

	Menu *pauseMenu2;

	vector<EventKeyboard::KeyCode> heldKeys;

	//CONSTRUCTOR & METHODS

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void updateScene(float interval);

	static cocos2d::Scene* createScene();

	virtual bool init();

	void selectMenu();

	void selectLevel1(Ref* pSender);
	void selectLevel2(Ref* pSender);
	void selectLevel3(Ref* pSender);
	void about(Ref* pSender);

	void startAnim2(float interval);

	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(StartScreen);

	StartScreen(void);
	virtual ~StartScreen(void);
};

#endif // _START_SCREEN_H_