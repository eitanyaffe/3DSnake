
#include "Snake.h"
#include "Textures.h"

#include <math.h>

#include <gl/glut.h>
#include<gl/gl.h>

Snake::Snake(int cubeSize)
: _state(ssIdle), _progress(0), _rotationProgress(0)
{}

void Snake::init(int cubeSize, int length)
{
	_state = ssIdle;
	_dir = OrthoDir(0,0,1);
	_up = OrthoDir(0,1,0);
	_nextDir = _dir;
	_nextUp = _up;

	_growCounter = length - 1;
	_chain.resize(1);
	_chain[0] = CubeCoord(cubeSize / 2, cubeSize / 2, 0);
	advance();
}

void Snake::render(bool snakeEyes)
{
	GLUquadricObj* quadObj = gluNewQuadric();

	for (int i = 0; i < _chain.size(); ++i)
	{
		float colors[4];
		colors[0] = 0.5;
		colors[1] = (0.5 * i) / _chain.size();
		colors[2] = 0.0;
		colors[3] = 1.0;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colors);
		colors[0] = 0.7;
		colors[1] = 0.7;
		colors[2] = 0.7;
		colors[3] = 1.0;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colors);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 100);

		glPushMatrix();
		
		CubeCoord nextCoord = getNextCoord(i);

		glTranslated((_chain[i].x*(1-_progress) + nextCoord.x*_progress)+ 0.5, 
					(_chain[i].y*(1-_progress) + nextCoord.y*_progress) + 0.5, 
					(_chain[i].z*(1-_progress) + nextCoord.z*_progress) + 0.5);

		if (_chain[i].z != nextCoord.z)
			glRotated(-90,0,0,1);
		else if (_chain[i].x != nextCoord.x)
			glRotated(-90,0,1,0);
		else 
			glRotated(90,1,0,0);

		// do not draw head when inside head!
		if (!snakeEyes || (i != 0) )
			drawSnakeSphere(quadObj);
	
		glPopMatrix();
	}
	gluDeleteQuadric(quadObj);
}

void Snake::setViewFromSnakeEyes()
{
	double eyex = _chain[0].x + _dir.x*_progress + 0.5;
	double eyey = _chain[0].y + _dir.y*_progress + 0.5;
	double eyez = _chain[0].z + _dir.z*_progress + 0.5;

	// F points from eye to target
	double Fx = _dir.x*(1-_rotationProgress) + _nextDir.x*_rotationProgress;
	double Fy = _dir.y*(1-_rotationProgress) + _nextDir.y*_rotationProgress;
	double Fz = _dir.z*(1-_rotationProgress) + _nextDir.z*_rotationProgress;
	double FLen = sqrt(Fx*Fx + Fy*Fy + Fz*Fz);
	Fx /= FLen; Fy /= FLen; Fz /= FLen;

	// U points up
	double Ux = _up.x*(1-_rotationProgress) + _nextUp.x*_rotationProgress;
	double Uy = _up.y*(1-_rotationProgress) + _nextUp.y*_rotationProgress;
	double Uz = _up.z*(1-_rotationProgress) + _nextUp.z*_rotationProgress;
	double ULen = sqrt(Ux*Ux + Uy*Uy + Uz*Uz);
	Ux /= ULen; Uy /= ULen; Uz /= ULen;

	// S is orthogonal to both
	double Sx = Fy*Uz - Fz*Uy;
	double Sy = Fz*Ux - Fx*Uz;
	double Sz = Fx*Uy - Fy*Ux;

	// set the ModelView matrix
	double m[16];
	m[0] = Sx;		m[4] = Sy;		m[8] = Sz;		m[12] = 0;
	m[1] = Ux;		m[5] = Uy;		m[9] = Uz;		m[13] = 0;
	m[2] = -Fx;		m[6] = -Fy;		m[10] = -Fz;	m[14] = 0;
	m[3] = 0;		m[7] = 0;		m[11] = 0;		m[15] = 1;

	glMultMatrixd(m);
	glTranslated(-eyex, -eyey, -eyez);
}

CubeCoord Snake::getNextCoord(int index)
{
	if (index > 0)
		return _chain[index-1];

	return CubeCoord(_chain[0].x + _dir.x, 
					 _chain[0].y + _dir.y, 
					 _chain[0].z + _dir.z);
}

OrthoDir Snake::getDir()
{
	return _dir;
}

void Snake::setDirection(OrthoDir dir)
{
	_dir = dir;
}

void Snake::advance()
{
	CubeCoord nextCoord = getNextCoord(0);

	// snake needs to grow, add one unit
	if (_growCounter > 0)
	{
		_growCounter--;
		_chain.push_back(CubeCoord());
	}

	int n = _chain.size();
	for (int i = n-1; i > 0; --i)
		_chain[i] = _chain[i-1];

	_chain[0] = nextCoord;

	_dir = _nextDir;
	_up = _nextUp;

	_state = ssIdle;
}

void Snake::setProgress(double value)
{
	_progress = value;
}

void Snake::setRotationProgress(double value)
{
	_rotationProgress = value;
}

void Snake::turnRight()
{
	// cannot act if already busy
	if (_state != ssIdle)
		return;
	_state = ssRotating;

	_nextDir = _dir.getOrtho(_up);
}

void Snake::turnLeft()
{
	// cannot act if already busy
	if (_state != ssIdle)
		return;
	_state = ssRotating;

	_nextDir = _up.getOrtho(_dir);
}

void Snake::turnUp()
{
	// cannot act if already busy
	if (_state != ssIdle)
		return;
	_state = ssRotating;

	_nextDir = _up;
	_nextUp = _dir.reverse();
}

void Snake::turnDown()
{
	// cannot act if already busy
	if (_state != ssIdle)
		return;
	_state = ssRotating;

	_nextDir = _up.reverse();
	_nextUp = _dir;
}

void Snake::rollRight()
{
	// cannot act if already busy
	if (_state != ssIdle)
		return;
	_state = ssRotating;

	_nextUp = _dir.getOrtho(_up);
}

void Snake::rollLeft()
{
	// cannot act if already busy
	if (_state != ssIdle)
		return;
	_state = ssRotating;

	_nextUp = _up.getOrtho(_dir);
}

OrthoDir OrthoDir::getOrtho(OrthoDir other)
{
	return OrthoDir(y*other.z - z*other.y,
					z*other.x - x*other.z,
					x*other.y - y*other.x);
}

OrthoDir OrthoDir::reverse()
{
	return OrthoDir(-x, -y, -z);
}

SnakeState Snake::getState()
{
	return _state;
}

void Snake::grow(int value)
{
	_growCounter += value;
}

bool Snake::isPartOfSnake(CubeCoord coord)
{
	for (int i = 0; i < _chain.size()-2; ++i)
	{
		CubeCoord chain_coord = _chain[i];
		if ((coord.x == chain_coord.x) &&
			(coord.y == chain_coord.y) &&
			(coord.z == chain_coord.z) )
			return true;
	}
	return false;
}

CubeCoord Snake::getHead()
{
	return _chain[0];
}

int Snake::getLength()
{
	return _chain.size();
}

