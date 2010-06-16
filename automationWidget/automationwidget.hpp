
/*	Program AutoMate: A musical automation tool
	Author: Harry van Haaren
	E-mail: harryhaaren@gmail.com
	Copyright (C) 2010 Harry van Haaren

	AutoMate is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	AutoMate is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with AutoMate.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef AUTOMATIONWIDGET
#define AUTOMATIONWIDGET

#include <gtkmm/drawingarea.h>

class AutomationWidget : public Gtk::DrawingArea
{
	public:
		AutomationWidget();
		~AutomationWidget();
		
		float getValue ();
		void update_time(float inTime);
		void setNumberOfBars(int setNumberOfBars);
		void set_points(std::vector<int> pointsIn);
		
		bool onScrollEvent	(GdkEventScroll *event);
		bool onMouseClick	(GdkEventButton *event);
		bool onMouseMove	(GdkEventMotion *event);
		bool on_expose_event(GdkEventExpose *event);
		bool onMouseButton1Down( GdkEventButton *event ) ;
		bool onMouseButton3Down( GdkEventButton *event ) ;
	
	protected:
		float time;
		int numberOfBars;
		int playheadPosition;				//	in pixels
		int width,height;
		float mouseX,mouseY;				// in pixels
		std::vector <float> horizontals;	// floating-points
		std::vector <float> verticals;		// floating-points
		
		bool redraw_all();
		bool redraw(int,int,int,int);
		int  timer_callback();
		std::string convertFloatToStr (float number);
		

};

#endif // AUTOMATIONWIDGET
