#include "Scene.h"

#include "apple.h"
#include "Textures.h"
#include "resource.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#include <gl/gl.h>
#endif

Scene::Scene(int cubeSize)
: _cubeSize(cubeSize), _snake(cubeSize), _showGrid(false), 
  _stepDuration(500), _snakeEyes(true), _level(0), 
  _highscore(0), _score(0), _showLevelLabel(false), _showCreditLabel(false)
{
	_cube = new StaticElement*[_cubeSize*_cubeSize*_cubeSize];
	for (int x = 0; x < _cubeSize; ++x)
		for (int y = 0; y < _cubeSize; ++y)
			for (int z = 0; z < _cubeSize; ++z)
				setElement(x, y, z, 0);

	// seed the random sequence
	srand(time(NULL));

	initTextures();
	initApple();
}

Scene::~Scene()
{
	deleteElements();

	delete[] _cube;
}

void Scene::deleteElement(int x, int y, int z)
{
	StaticElement* element = getElement(x, y, z);
	if (element)
	{
		delete element;
		setElement(x, y, z, 0);
	}
}

void Scene::deleteElements()
{
	for (int x = 0; x < _cubeSize; ++x)
		for (int y = 0; y < _cubeSize; ++y)
			for (int z = 0; z < _cubeSize; ++z)
				deleteElement(x, y, z);
}

StaticElement* Scene::getElement(int x, int y, int z)
{
	return _cube[x + y*_cubeSize + z*_cubeSize*_cubeSize];
}

void Scene::setElement(int x, int y, int z, StaticElement* element)
{
	_cube[x + y*_cubeSize + z*_cubeSize*_cubeSize] = element;
}

int randomInt(int from, int to)
{
	double x = rand();
	x /= RAND_MAX;
	x *= to-from;
	x += from;
	return floor(x + 0.5);
}

void Scene::addStaticElement(StaticElement* element)
{
	while (true)
	{
		int x = randomInt(0, _cubeSize-1);
		int y = randomInt(0, _cubeSize-1);
		int z = randomInt(0, _cubeSize-1);
		
		// keep space for snake
		if ((x == _cubeSize/2) && (y == _cubeSize/2))
			continue;

		StaticElement* otherElement = getElement(x,y,z);
		if (otherElement == 0)
		{
			setElement(x, y, z, element);
			element->setLocation(x,y,z);
			break;
		}
	}

}

void Scene::restartGame(int level)
{
	_levelLabelStartTime = clock();
	_showLevelLabel = true;
	_showCreditLabel = false;

	if (level == 0)
		_score = 0;
	_level = level;

	deleteElements();
	startGame(_numberOfBricks + 3*_level, 
			  _numberOfRedApples + _level, 
			  _numberOfGreenApples + _level);
	_pause = false;
	_gameOver = false;
}

void Scene::init(int numberOfBricks,
			   int numberOfRedApples, 
			   int numberOfGreenApples)
{
	_numberOfBricks = numberOfBricks;
	_numberOfRedApples = numberOfRedApples;
	_numberOfGreenApples = numberOfGreenApples;
}

void Scene::startGame(int numberOfBricks,
					  int numberOfRedApples, int numberOfGreenApples)
{
	_appleCount = numberOfRedApples + numberOfGreenApples;
	_stepDuration = 40000 - _level*5;

	_flyingEyeX = 22;
	_flyingEyeY = 5;
	_flyingEyeZ = 5;

	_pause = true;
	_gameOver = true;

	int i;
	for (i = 0; i < numberOfBricks; ++i)
	{
		StaticElement* element = new StaticElement(eBrick);
		addStaticElement(element);
	}
	for (i = 0; i < numberOfRedApples; ++i)
	{
		StaticElement* element = new StaticElement(eRedApple);
		addStaticElement(element);
	}
	for (i = 0; i < numberOfGreenApples; ++i)
	{
		StaticElement* element = new StaticElement(eGreenApple);
		addStaticElement(element);
	}

	// start with snake of length 3
	_snake.init(_cubeSize, 6 + 4*_level);

	clock_t c = clock();
	_startStepTime = c;
	_endStepTime = c + _stepDuration;
}

void outputGL(double x, double y, char *string, bool center = true, float bgAlpha = 0.3f)
{
	// Save OpenGL state
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	// Set up orthographic projection for 2D text rendering
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	// Disable features that might interfere with text rendering
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	// Calculate text width if centering is needed
	int textWidth = 0;
	if (center) {
		for (int i = 0; string[i] != '\0'; i++) {
			textWidth += glutBitmapWidth(GLUT_BITMAP_9_BY_15, string[i]);
		}
		x -= textWidth / 2.0;
	}
	
	// Draw semi-transparent background
	if (bgAlpha > 0) {
		glColor4f(0.0f, 0.0f, 0.0f, bgAlpha);
		glBegin(GL_QUADS);
		glVertex2f(x - 5, y - 5);
		glVertex2f(x + textWidth + 5, y - 5);
		glVertex2f(x + textWidth + 5, y + 15);
		glVertex2f(x - 5, y + 15);
		glEnd();
	}
	
	// Set text color
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	// Position text
	glRasterPos2f(x, y);
	
	// Render text
	for (int i = 0; string[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
	}
	
	// Restore OpenGL state
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glPopAttrib();
}

void Scene::setMaterialColor(float r, float g, float b, float a)
{
	float colors[4];
	colors[0] = r;
	colors[1] = g;
	colors[2] = b;
	colors[3] = a;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colors);
	colors[0] = 1;
	colors[1] = 1;
	colors[2] = 1;
	colors[3] = a;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colors);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 2);
}

void Scene::positionLights()
{
	GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_position[] = { 
		static_cast<GLfloat>(_cubeSize/2), 
		static_cast<GLfloat>(_cubeSize-2), 
		static_cast<GLfloat>(_cubeSize/2), 
		1.0f 
	};

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);

	// light on help plate
	if (_pause && _snakeEyes)
	{
		glPushMatrix();
		glLoadIdentity();
		GLfloat light_ambient2[] = { 1, 1, 1, 1 }; 
		GLfloat light_diffuse2[] = { 1, 1, 1, 1 }; 
		GLfloat light_specular2[] = { 1, 1, 1, 1 }; 
		GLfloat light_position2[] = { 0, 0, -0.24, 1 };
		GLfloat light_dir2[] = { 0, 0, 1 };

		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_dir2);
		glLighti(GL_LIGHT2, GL_SPOT_CUTOFF, 90);

		glPopMatrix();
	}

	if (!_snakeEyes)
	{
		glPushMatrix();
		glLoadIdentity();
		GLfloat light_ambient3[] = { .2, .2, .2, 1 }; 
		GLfloat light_diffuse3[] = { .4, .4, .4, 1 }; 
		GLfloat light_specular3[] = { 0, 0, 0, 1 }; 
		GLfloat light_position3[] = { 0, 0, 0, 1 };

		glEnable(GL_LIGHT3);
		glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
		glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
		glLightfv(GL_LIGHT3, GL_POSITION, light_position3);

		glPopMatrix();
	}
	else
		glDisable(GL_LIGHT3);
}

void Scene::render()
{
	// set view
	if (_snakeEyes)
		_snake.setViewFromSnakeEyes();
	else
		gluLookAt(_cubeSize+4, _cubeSize/2, _cubeSize/2,
				  _cubeSize/2, _cubeSize/2, _cubeSize/2,
				  0,1,0);

	// lights
	positionLights();

	// walls
	setMaterialColor(0.2, 0, 1, 1);
	drawWalls(_cubeSize, !_snakeEyes);

	// snake
	_snake.render(_snakeEyes);

	// bricks and apples
	for (int x = 0; x < _cubeSize; ++x)
		for (int y = 0; y < _cubeSize; ++y)
			for (int z = 0; z < _cubeSize; ++z)
			{
				StaticElement* element = getElement(x, y, z);
				if (element)
					element->render();
			}

	// text
	if (_snakeEyes)
	{
		char scoreStr[100];
		snprintf(scoreStr, sizeof(scoreStr), 
			"Score: %d   Speed: %.0f   Length: %d   High Score: %d", 
			_score, 1000.0/_stepDuration*60, _snake.getLength(), _highscore);
		setMaterialColor(1, 1, 0, 1);
		outputGL(25, 570, scoreStr, false, 0.7f);

		// Add permanent level title in top-right corner
		char levelTitle[30];
		sprintf(levelTitle, "Level %d", _level + 1);
		setMaterialColor(0, 1, 1, 1);
		outputGL(700, 570, levelTitle, false, 0.7f);
	}

	if (_showLevelLabel && !_pause && _snakeEyes)
	{
		char newLevel[30];
		sprintf(newLevel, "LEVEL %d (%d GREENS, %d REDS)", 
			_level+1, _numberOfGreenApples+_level, _numberOfRedApples+_level);
		setMaterialColor(1, 0.5, 0, 1);
		outputGL(400, 300, newLevel, true, 0.7f);
	}
	
	if (_showCreditLabel && !_showLevelLabel && !_pause && _snakeEyes)
	{
		char credit[30];
		sprintf(credit, "%d POINTS", _credit);
		setMaterialColor(1,1,1,1);
		outputGL(392, 300, credit);
	}

	// grid
	setMaterialColor(0.8, 0.5, 1, 0.3);
	if (_showGrid && _snakeEyes)
		drawGrid();

	if (_pause && _snakeEyes)
	{
		setMaterialColor(1, 1, 1, 0.6);
		drawHelp();
	}
}

void Scene::updateScene()
{
	if (_pause)
		return;

	clock_t c = clock();

	// reached end of step
	if (c > _endStepTime)
	{
		_startStepTime = c;
		_endStepTime = c + _stepDuration;

		advanceSnake();
	}
	_progress = (1.0 * (c - _startStepTime)) / (_endStepTime - _startStepTime);

	if (_snake.getState() == ssRotating)
		_rotateProgress = (1.0 * (c - _startRotationTime)) / (_endStepTime - _startRotationTime);
	else
		_rotateProgress = 0;
	
	_snake.setProgress(_progress);
	_snake.setRotationProgress(_rotateProgress);

	if (_showLevelLabel && (_levelLabelStartTime + 200000 < c))
		_showLevelLabel = false;

	if (_showCreditLabel && (_creditStartTime + 150000 < c))
		_showCreditLabel = false;

	if (_gameOver)
	{
		_sound.playWallSound();  // Use wall sound for game over
		return;
	}
}

void Scene::advanceSnake()
{
	_snake.advance();

	CubeCoord nextCoord = _snake.getNextCoord(0);
	if ( (nextCoord.x < 0) || (nextCoord.y < 0) || (nextCoord.z < 0) ||
		 (nextCoord.x >= _cubeSize) || (nextCoord.y >= _cubeSize) || (nextCoord.z >= _cubeSize) ||
		 _snake.isPartOfSnake(nextCoord))
	{
		// Visual feedback instead of sound
		glFlush();
		
		_pause = true;
		_gameOver = true;
		return;
	}

	// check if we hit something
	StaticElement* element = getElement(nextCoord.x, nextCoord.y, nextCoord.z);
	if (element != NULL)
	{
		switch (element->getType())
		{
		case eBrick:
			_sound.playWallSound();
			_gameOver = true;
			_pause = true;
			// Visual feedback instead of sound
			glFlush();
			break;
		case eGreenApple:
			_sound.playAppleSound();
			_score += 3;
			deleteElement(nextCoord.x, nextCoord.y, nextCoord.z);
			_appleCount--;
			_snake.grow(2);
			_stepDuration *= 0.98;
			_showCreditLabel = true;
			_credit = 3;
			_creditStartTime = clock();
			// Visual feedback instead of sound
			glFlush();
			break;
		case eRedApple:
			_sound.playAppleSound();
			_score += 5;
			deleteElement(nextCoord.x, nextCoord.y, nextCoord.z);
			_appleCount--;
			_snake.grow(2);
			_stepDuration *= 0.96;
			_showCreditLabel = true;
			_credit = 5;
			_creditStartTime = clock();
			// Visual feedback instead of sound
			glFlush();
			break;
		}
	}

	if (_score > _highscore)
		_highscore = _score;

	if (_appleCount == 0)
	{
		restartGame(_level+1);
	}
}

void Scene::toggleGrid()
{
	_showGrid = !_showGrid;
}

void Scene::toggleSnakeEyes()
{
	_snakeEyes = !_snakeEyes;
}

void Scene::togglePause()
{
	if (_gameOver)
		return;

	if (_pause)
	{
		clock_t c = clock();

		_startStepTime = c - _progress*_stepDuration;
		_endStepTime = _startStepTime + _stepDuration;

		double deltaRotation = _endStepTime - _startRotationTime;
		_startRotationTime = c - _rotateProgress*deltaRotation;
	}
	_pause = !_pause;
}

void Scene::drawGrid()
{
	glBegin(GL_LINES);
	{
		int x, y, z;
		for (x = 0; x <= _cubeSize; ++x)
			for (y = 0; y <= _cubeSize; ++y)
			{
				glNormal3f(0,0,1);
				glVertex3f(x, y, 0);
				glVertex3f(x, y, _cubeSize);
			}
		for (x = 0; x <= _cubeSize; ++x)
			for (z = 0; z <= _cubeSize; ++z)
			{
				glNormal3f(0,1,0);
				glVertex3f(x, 0, z);
				glVertex3f(x, _cubeSize, z);
			}
		for (z = 0; z <= _cubeSize; ++z)
			for (y = 0; y <= _cubeSize; ++y)
			{
				glNormal3f(1,0,0);
				glVertex3f(0, y, z);
				glVertex3f(_cubeSize, y, z);
			}
	}
	glEnd();
}

bool Scene::startRotation()
{
	if (_gameOver || _pause)
		return false;

	if (_snake.getState() == ssRotating)
		return false;

	clock_t c = clock();

	_startRotationTime = c;
	return true;
}

void Scene::turnLeft()
{
	if (startRotation())
		_snake.turnLeft();
}

void Scene::turnRight()
{
	if (startRotation())
		_snake.turnRight();
}

void Scene::turnUp()
{
	if (startRotation())
		_snake.turnUp();
}

void Scene::turnDown()
{
	if (startRotation())
		_snake.turnDown();
}

void Scene::rollLeft()
{
	if (startRotation())
		_snake.rollLeft();
}

void Scene::rollRight()
{
	if (startRotation())
		_snake.rollRight();
}

