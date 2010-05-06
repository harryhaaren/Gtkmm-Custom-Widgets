#ifndef CANVASOBJECT
#define CANVASOBJECT

#include <iostream>

#include <gtkmm.h>

class CanvasObject
{
	public:
		CanvasObject();
		~CanvasObject();
		
		void setInfo(int inX,int inY,int inHeight,int inWidth);
		
		int getX();
		int getY();
		int getHeight();
		int getWidth();
	
	protected:
		int x;
		int y;
		int height;
		int width;

};

#endif // JACKLABEL
