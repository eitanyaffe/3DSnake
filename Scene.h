#ifndef _Scene_
#define _Scene_

#include <time.h>
#include <vector>
using namespace std;

#include "Snake.h"
#include "StaticElement.h"
#include "Sound.h"

//////////////////////////////////////////////////////////////////////
//
// Scene.h header file
//
//////////////////////////////////////////////////////////////////////

class Scene 
{
private:
	clock_t _startStepTime;
	clock_t _endStepTime;
	clock_t  _stepDuration;
	double _progress;

	clock_t _startRotationTime;
	double _rotateProgress;

	bool _pause;
	bool _gameOver;
	bool _snakeEyes;

	Snake _snake;
	int _cubeSize;
	StaticElement** _cube;
	
	int _highscore;
	int _score;
	int _appleCount;
	int _level;
	bool _showGrid;

	clock_t _levelLabelStartTime;
	bool _showLevelLabel;

	clock_t _creditStartTime;
	bool _showCreditLabel;
	int _credit;

	double _flyingEyeX, _flyingEyeY, _flyingEyeZ;

	int _numberOfBricks, _numberOfRedApples, _numberOfGreenApples;

	Sound _sound;

	void setMaterialColor(float r, float g, float b, float a);
	void drawGrid();
	void positionLights();

	void advanceSnake();
	
	// returns true iff rotation can be started
	bool startRotation();

	void deleteElement(int x, int y, int z);
	void deleteElements();

	void addStaticElement(StaticElement* element);

	StaticElement* getElement(int x, int y, int z);
	void setElement(int x, int y, int z, StaticElement* element);

public:
	Scene(int cubeSize); 
	virtual ~Scene();

	void init (int numberOfBricks,
				   int numberOfRedApples, 
				   int numberOfGreenApples);
	
	void startGame(int numberOfBricks,
				   int numberOfRedApples, 
				   int numberOfGreenApples);
	void restartGame(int level);

	void updateScene();
	void render();

	void toggleGrid();
	void toggleSnakeEyes();
	void togglePause();

	void turnLeft();
	void turnRight();
	void turnUp();
	void turnDown();
	void rollLeft();
	void rollRight();
};

#endif