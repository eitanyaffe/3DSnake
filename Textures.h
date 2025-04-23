#ifndef _Textures_
#define _Textures_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#include <gl/gl.h>
#endif

// code taken from NeHe tutorial

void initTextures();

void drawWalls(double cubeSize, bool openFront);
void drawHelp();
void drawBrick();
void drawApple();
void drawSnakeSphere(GLUquadricObj* quadObj);

#endif