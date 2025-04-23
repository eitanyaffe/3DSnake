#ifndef _Snake_
#define _Snake_

//////////////////////////////////////////////////////////////////////
//
// Snake.h header file
//
//////////////////////////////////////////////////////////////////////

#include <vector>
using namespace std;

struct CubeCoord
{
	int x, y, z;
	CubeCoord() {};
	CubeCoord(int xv, int yv, int zv) : x(xv), y(yv), z(zv) {}; 
};

struct OrthoDir 
{
	int x, y, z;
	OrthoDir() {};
	OrthoDir(int xv, int yv, int zv) : x(xv), y(yv), z(zv) {};

	// using right-hand-rule
	OrthoDir getOrtho(OrthoDir other);
	OrthoDir reverse();
};

enum SnakeState { ssIdle, ssRotating};

class Snake 
{
private:
	OrthoDir _dir;
	OrthoDir _up;

	OrthoDir _nextDir;
	OrthoDir _nextUp;

	// between 0 and 1
	double _progress;
	double _rotationProgress;

	SnakeState _state;
	int _growCounter;

	vector< CubeCoord > _chain;

public:
	// ctor
	Snake(int cubeSize);

	// scene functions
	void init(int cubeSize, int length);
	void advance();
	void render(bool snakeEyes);
	void setViewFromSnakeEyes();
	void setProgress(double value);
	void setRotationProgress(double value);

	OrthoDir getDir();
	CubeCoord getNextCoord(int index);
	void setDirection(OrthoDir);

	SnakeState getState();
	int getLength();

	void grow(int value);
	bool isPartOfSnake(CubeCoord coord);
	CubeCoord getHead();
	// user functions

	void turnLeft();
	void turnRight();
	void turnUp();
	void turnDown();
	void rollLeft();
	void rollRight();
};

#endif