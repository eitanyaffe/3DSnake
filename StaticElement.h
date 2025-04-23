#ifndef _StaticElement_
#define _StaticElement_

enum ElementType {eBrick, eRedApple, eGreenApple};

class StaticElement
{
private:
	int _x, _y, _z;
	ElementType _type;

protected:
	void buildElement();

public:
	StaticElement(ElementType type);

	void setLocation(int x, int y, int z);

	ElementType getType();
	void render();
};

#endif