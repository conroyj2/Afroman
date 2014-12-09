#include "GameScene2.h"
#include "StartScreen.h"
#include "globals.h"
#include "SimpleAudioEngine.h"


Scene* GameScene2::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene2::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene2::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//preload the sound of each drum

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("hihat.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("snare.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bass.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("cymbal.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("tom1.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("tom2.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("tom3.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("error.wav");

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("song.mp3"); //preload the background music file

	auto director = Director::getInstance(); //creates the director

	wsize = director->getOpenGLView()->getFrameSize(); //finds the size of the window
	center = Point(wsize.width / 2, wsize.height / 2); //finds the coordinates of the center point

	//tilemap of the notes

	level = new Level(); //creates a new level called "level"
	level->loadMap("level2.tmx"); //loads the map
	level->getMap()->setAnchorPoint(Point(0, 0)); //sets the point that determines the position to be the bottom-lefthand corner
	level->getMap()->setPosition((-3500) - level->getMap()->getContentSize().width, 0); //sets the initial position of the map
	this->addChild(level->getMap(), 10); //adds the tilemap to the scene

	//creates the score label with many parameters
	scoreLabel1 = LabelTTF::create("SCORE: " + to_string(score), "8-Bit Madness", 40, CCSizeMake(245, 32), kCCTextAlignmentCenter);
	scoreLabel1->setPosition(422, 600); //sets position of the label
	this->addChild(scoreLabel1, 10000); //adds label to the scene

	//the background image as a sprite
	bg_sprite = Sprite::create("level2bg.png"); //creates the sprite using the filename
	bg_sprite->setPosition(0, center.y); //sets the position
	this->addChild(bg_sprite, 0); //adds the sprite to the scene

	//creates the camera
	cameraTarget = Sprite::create(); //creates the target as a sprite
	cameraTarget->setPosition(bg_sprite->getPosition()); //sets the position of the target
	this->addChild(cameraTarget); //adds the camera target to the scene
	camera = Follow::create(cameraTarget, Rect::ZERO); //camera will follow the camera target
	this->runAction(camera);


	//Menus and Menu Items

	//this is a main menu which is always visible and consists only of the pause button which will open a new menu when clicked
	menu = Menu::create();
	pauseButton = MenuItemImage::create("pauseButton.png", "pauseButtonSelected.png", this, menu_selector(GameScene2::pauseMenu));
	menu->addChild(pauseButton);
	menu->setPosition(cameraTarget->getPositionX() - (.47 * wsize.width), cameraTarget->getPositionY() + (.45 * wsize.height));
	this->addChild(menu, 10000);

	//this is an image showing the controls of this level
	controls = Sprite::create("controls.png");
	controls->setPosition(cameraTarget->getPosition());
	this->addChild(controls, 10000);
	
	//this is an image that serves as the background of the pause menu
	pauseMenubg = Sprite::create("pauseMenubg.png");
	pauseMenubg->setOpacity(0); //sets the opacity to 0 so that it will fade in from 0
	this->addChild(pauseMenubg, 10000);

	//this is an image that serves as the background of the "you win" menu
	youWinMenubg = Sprite::create("score.png");
	youWinMenubg->setPosition(cameraTarget->getPosition());
	youWinMenubg->setOpacity(0); //sets the opacity to 0 so that it will fade in from 0
	this->addChild(youWinMenubg, 10000);

	//creates another score label that is invisible and will only become visible at when the level is over and the "you win" menu is visible
	scoreLabel2 = LabelTTF::create(to_string(score), "8-Bit Madness", 60, CCSizeMake(500, 40), kCCTextAlignmentCenter);
	scoreLabel2->setPosition(cameraTarget->getPosition() + ccp(0, 155));
	scoreLabel2->setVisible(false);
	this->addChild(scoreLabel2, 10000);


	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("song.mp3", true); //starts playing the background music

	this->schedule(schedule_selector(GameScene2::updateScene)); //sets the method updateScene to run every second

	return true;
}

void GameScene2::updateScene(float delta){

	//if the first note is entering the screen, fade the controls image out
	if (level->getMap()->getPositionX() <= ((-center.x) - level->getMap()->getContentSize().width)){
		FadeOut* fadeOut = FadeOut::create(.25);
		controls->runAction(fadeOut);
	}

	//if the level is over, display the "you win" menu
	if (level->getMap()->getPositionX() >= (center.x)){
		youWinMenu();
	}

	//if the level is not paused, update the scene
	if (!paused){
		scoreLabel1->setString("SCORE: " + to_string(score)); //update the score to show the current score

		level->getMap()->setPositionX(level->getMap()->getPositionX() + (8.2)); //move the map each update for the notes to move across the screen


		//checking to see if there are notes at the position where they need to be hit for each drum

		string drums[] = { "cymbal", "hihat", "snare", "tom1", "tom2", "tom3", "bass" };

		bool *bools[] = { &cymbal, &hihat, &snare, &tom1, &tom2, &tom3, &bass };

		for (int i = 0; i < 7; i++){
			int k = (i + 1) * 2;
			for (int j : level->getNotes(drums[i], k)){
				if ((level->getMap()->getPositionX() <= (center.x + 5 - level->getMap()->getTileSize().width - (j * level->getMap()->getTileSize().width))) &&
					(level->getMap()->getPositionX() >= (center.x - 5 - (j * level->getMap()->getTileSize().width) - (3 * level->getMap()->getTileSize().width)))){

 					*bools[i] = true;
					break;
				}
				else {
					*bools[i] = false;
				}
			}
		}
	}
}

void GameScene2::pauseMenu(Ref *psender){
	//only open the pause menu if the game is not already paused
	if (!paused){
		paused = true;

		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic(); //pause the background music

		//create each menu item, set their positions, and add them to an array
		Vector <MenuItem*> menuItems;

		MenuItem* resumeButton = MenuItemImage::create("resumeButton.png", "resumeButton.png", this, menu_selector(GameScene2::resumeGame));
		resumeButton->setPosition(ccp(0, 125));
		menuItems.pushBack(resumeButton);
		MenuItem* restartButton = MenuItemImage::create("restartButton.png", "restartButton.png", this, menu_selector(GameScene2::restart));
		restartButton->setPosition(ccp(0, 25));
		menuItems.pushBack(restartButton);
		MenuItem* nextButton = MenuItemImage::create("nextButton.png", "nextButton.png", this, menu_selector(GameScene2::selectLevel));
		nextButton->setPosition(ccp(0, -75));
		menuItems.pushBack(nextButton);
		MenuItem* exitButton = MenuItemImage::create("exitButton.png", "exitButton.png", this, menu_selector(GameScene2::menuCloseCallback));
		exitButton->setPosition(ccp(0, -175));
		menuItems.pushBack(exitButton);

		//create the menu with the menu items in the array
		pauseMenu2 = Menu::createWithArray(menuItems);
		pauseMenu2->setPosition(cameraTarget->getPosition());
		this->addChild(pauseMenu2, 10000);

		//fades in the background of the pause menu
		FadeIn *fadeIn = FadeIn::create(.25);
		pauseMenubg->runAction(fadeIn);
		pauseMenubg->setPosition(cameraTarget->getPosition());
	}
}

void GameScene2::youWinMenu(){
	if (!paused){
		paused = true;

		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();

		paused = true;

		Vector <MenuItem*> menuItems;

		MenuItem* restartButton = MenuItemImage::create("restartButton.png", "restartButton.png", this, menu_selector(GameScene2::restart));
		restartButton->setPosition(ccp(0, 55));
		menuItems.pushBack(restartButton);
		MenuItem* nextButton = MenuItemImage::create("nextButton.png", "nextButton.png", this, menu_selector(GameScene2::selectLevel));
		nextButton->setPosition(ccp(0, -45));
		menuItems.pushBack(nextButton);
		MenuItem* exitButton = MenuItemImage::create("exitButton.png", "exitButton.png", this, menu_selector(GameScene2::menuCloseCallback));
		exitButton->setPosition(ccp(0, -145));
		menuItems.pushBack(exitButton);

		int totalNotes = level->getNotes("hihat", 4).size() + level->getNotes("cymbal", 2).size() + level->getNotes("snare", 6).size()
			+ level->getNotes("tom1", 8).size() + level->getNotes("tom2", 10).size() + level->getNotes("tom3", 12).size() + level->getNotes("bass", 14).size();

		int percentage = (int)(100 * score / totalNotes);

		if (percentage < 0){
			scoreLabel2->setString(to_string(score) + "/" + to_string(totalNotes) + " = BAD");
		}
		else{
			scoreLabel2->setString(to_string(score) + "/" + to_string(totalNotes) + " = " + to_string(percentage) + "%");
		}

		scoreLabel2->setVisible(true);

		pauseMenu2 = Menu::createWithArray(menuItems);
		pauseMenu2->setPosition(cameraTarget->getPosition());
		pauseMenu2->setGlobalZOrder(10000);
		this->addChild(pauseMenu2, 10000);

		FadeIn *fadeIn = FadeIn::create(.5);
		youWinMenubg->runAction(fadeIn);
		youWinMenubg->setPosition(cameraTarget->getPosition());
	}
}

void GameScene2::resumeGame(Ref *psender){

	pauseMenu2->removeFromParent(); //remove the menu when the game is not pause anymore

	//fade out the background image of the menu
	FadeOut *fadeOut = FadeOut::create(.25);
	pauseMenubg->runAction(fadeOut);

	paused = false; //unpause the game

	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic(); //resume the background music
}

void GameScene2::restart(Ref *psender){

	//creates a new scene of the same level and restarts it from the beginning
	auto newScene = GameScene2::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, newScene));
}

void GameScene2::selectLevel(Ref *psender){

	//replaces current scene with a new scene of the next level
	auto newScene = StartScreen::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, newScene));
}

void GameScene2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	//if the game is not paused, it checks to see if keys are pressed
	//for each key pressed, if the corresponding note is there, the sound will be played and the score will go up
	//else if the controls menu is still visible, the sound plays
	//else, the score goes down and an error sound is played

	if (!paused){
		if (keyCode == SPACEBAR){

			if (bass){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("bass.wav");
				score++;
			}
			else if (controls->getOpacity() > 0){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("bass.wav");
			}
			else{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("error.wav");
				score--;
			}
		}
		if (keyCode == SNARE){
			if (snare){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("snare.wav");
				score++;
			}
			else if (controls->getOpacity() > 0){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("snare.wav");
			}
			else{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("error.wav");
				score--;
			}
		}
		if (keyCode == HI_HAT){
			if (hihat){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("hihat.wav");
				score++;
			}
			else if (controls->getOpacity() > 0){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("hihat.wav");
			}
			else{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("error.wav");
				score--;
			}
		}
		if (keyCode == TOM1){
			if (tom1){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("tom1.wav");
				score++;
			}
			else if (controls->getOpacity() > 0){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("tom1.wav");
			}
			else{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("error.wav");
				score--;
			}
		}
		if (keyCode == TOM2){
			if (tom2){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("tom2.wav");
				score++;
			}
			else if (controls->getOpacity() > 0){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("tom2.wav");
			}
			else{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("error.wav");
				score--;
			}
		}
		if (keyCode == TOM3){

			if (tom3){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("tom3.wav");
				score++;
			}
			else if (controls->getOpacity() > 0){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("tom3.wav");
			}
			else{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("error.wav");
				score--;
			}
		}
		if (keyCode == CYMBAL){
			if (cymbal){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cymbal.wav");
				score++;
			}
			else if (controls->getOpacity() > 0){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cymbal.wav");
			}
			else{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("error.wav");
				score--;
			}
		}

		if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()){
			heldKeys.push_back(keyCode);
		}
	}
}

void GameScene2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}

void GameScene2::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

GameScene2::GameScene2(void)
{
	setKeyboardEnabled(true);
}
GameScene2::~GameScene2(void)
{
}
