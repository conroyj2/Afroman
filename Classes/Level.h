#ifndef  _LEVEL_H_
#define  _LEVEL_H_

#include "cocos2d.h"
#include <vector>

using namespace std;
USING_NS_CC;

class Level : public Object
{
public:

	TMXTiledMap *map;

	void loadMap(const char* name);
	TMXTiledMap * getMap();

	Point tileCoordinateToPosition(Point point);
	Point positionToTileCoordinate(Point point);
	vector<Rect> getTiles(Point point, int fromX, int fromY, string filename);
	vector<Rect> getWalls(string filename);
	vector<int> getNotes(string filename, int yValue);

	Level(void);
	virtual ~Level(void);
};

#endif // _LEVEL_H_