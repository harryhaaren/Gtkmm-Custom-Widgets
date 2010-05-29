#include "canvasobject.hpp"

CanvasObject::CanvasObject()
{
}

CanvasObject::~CanvasObject()
{
}

void CanvasObject::setInfo(int inX,int inY,int inHeight,int inWidth)
{
	x = inX;
	y = inY;
	height = inHeight;
	width = inWidth;
}

int CanvasObject::getX()
{
	return x;
}
int CanvasObject::getY()
{
	return y;
}
int CanvasObject::getHeight()
{
	return height;
}
int CanvasObject::getWidth()
{
	return width;
}
