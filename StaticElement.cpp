#include "StaticElement.h"
#include "apple.h"
#include "Textures.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#include <gl/gl.h>
#endif

StaticElement::StaticElement(ElementType type) :
_x(0), _y(0), _z(0), _type(type)
{}

void StaticElement::setLocation(int x, int y, int z)
{
	_x = x;
	_y = y;
	_z = z;
}

ElementType StaticElement::getType()
{
	return _type;
}

void StaticElement::render()
{
		glPushMatrix();
		glTranslated(_x, _y, _z);

		buildElement();

		glPopMatrix();

}

void StaticElement::buildElement()
{
	float params[4];
	params[0] = 0;
	params[1] = 0;
	params[2] = 0;
	params[3] = 1;

	switch (_type)
	{
	case eRedApple:
		// set red ambient and diffuse material
		params[0] = 1.0f;
		params[1] = 0.0f;
		params[2] = 0.0f;
		params[3] = 1.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, params);

		// set specular and shininess
		params[0] = 0.05f;
		params[1] = 0.05f;
		params[2] = 0.05f;
		params[3] = 1.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, params);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 1);

		drawApple();
		break;

	case eGreenApple:
		params[1] = 0.4;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, params);
		params[0] = 0.05;
		params[1] = 0.05;
		params[2] = 0.05;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, params);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 1);
		drawApple();
		break;

	case eBrick:
		params[0] = 0.7;
		params[1] = 0.7;
		params[2] = 0.7;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, params);
		params[0] = 0.0;
		params[1] = 0.0;
		params[2] = 0.0;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, params);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);
		drawBrick();
		break;
	}
}