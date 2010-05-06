#ifndef MODULARCANVAS
#define MODULARCANVAS

#include <vector>
#include <iostream>
#include <gtkmm.h>

#include "canvasobject.hpp"

class ModularCanvas : public Gtk::DrawingArea
{
	public:
		ModularCanvas();
		~ModularCanvas();
		void redraw();
	
	protected:
		
		std::vector<CanvasObject> objectVector;
		
		// Button & Expose event.
		//bool on_button_press_event(GdkEventButton* event);
		bool on_expose_event(GdkEventExpose* event);
		bool onScrollEvent(GdkEventScroll *event);
};

#endif // ModularCanvas
