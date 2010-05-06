#include "radialview.hpp"
#include <iostream>
#include <cairomm/context.h>
#include <sstream>

// To Convert Floats to STR. Used to display the Value of the Widget on the widget.
std::string ConvertFloatToStr (float number){
     std::ostringstream buff;
     buff<<number;
     return buff.str();
}


RadialView::RadialView()
{
	angle = 0 ; 
	bpm = 120;
	totalTime = (( 60.0 / bpm) * 8) * 1000 ;  // *1000 from seconds to milliSeconds
	
	// Gives Exposure & Button presses to the widget.
	add_events(Gdk::EXPOSURE_MASK);
	add_events(Gdk::POINTER_MOTION_MASK );
	
	knobType = POSITIVE;
	
	// set default widget size
	set_size_request(100,100);
	
	RadialView::set_angle(0.0);
}

RadialView::~RadialView()
{
}

void RadialView::set_angle(float angleToSet)
{
	// For Displaying Value on Widget
	angleStr = ConvertFloatToStr (angleToSet*127);
	
	angle = (angleToSet * (3.1415*2)*3/4) - 3.1415*2 *3/8 ;
	RadialView::update();
}

bool RadialView::update()
{
	angle = angle;// + 0.1; //0.1
	//std::cout << angle << std::endl;
	
	// force our program to redraw the entire widget.
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
	    Gdk::Rectangle r(0, 0, get_allocation().get_width(),
	            get_allocation().get_height());
	    win->invalidate_rect(r, false);
	}
	
	// stay connected
	return true;
}

bool RadialView::on_expose_event(GdkEventExpose* event)
{
	if (knobType == POSITIVE )
	{
		// This is where we draw on the window
		Glib::RefPtr<Gdk::Window> window = get_window();
		
		if(window)    // Only run if Window does exist
		{
			Gtk::Allocation allocation = get_allocation();
			const int width = allocation.get_width();
			const int height = allocation.get_height();
			
			// coordinates for the center of the window
			int xc, yc;
			xc = width / 2;
			yc = height / 2;
			
			Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
			
			// set line Width
			cr -> set_line_width(6.0);
			cr -> set_line_join ( Cairo::LINE_JOIN_ROUND);
			cr -> set_line_cap  ( Cairo::LINE_CAP_ROUND );
			
			
			// clip to the area indicated by the expose event so that we only redraw
			// the portion of the window that needs to be redrawn
			cr->rectangle(event->area.x, event->area.y,
			        event->area.width, event->area.height);
			cr->clip();
			
			// Draw Main Arc
			cr -> set_line_width(9.0);
			cr -> set_source_rgb (0.1,0.1,0.1);
			cr -> arc(xc,yc,( (xc + yc)/2) -17, 0 ,2 * 3.1415);
			cr -> stroke();
			
			// Arc total Run
			cr -> set_line_width(6.0);
			cr -> set_source_rgb (0.3,1.0,0.0);
			cr -> arc(xc,yc,( (xc + yc)/2) -17, 2.35  , 0.8  );
			cr -> stroke();
			
			// Arc Angle Value
			cr -> set_line_width(6.0);
			cr -> set_source_rgb (1.0,0.0,0.0);
			cr -> arc(xc,yc,( (xc + yc)/2) -17, 2.35,(angle-3.1415*3/4)+0.8);
			cr -> stroke();
			
			// Draw "Function" Text
			cr -> set_source_rgb ( 0.1,0.1,0.1);
			cr -> select_font_face ("Impact" , Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
			cr -> set_font_size ( xc/4 );
			cr -> move_to ( xc-(xc/4) , yc-(yc/4) );
			cr -> show_text ("VOL");
			
			// Draw Variable Text
			cr -> set_source_rgb ( 0.1,0.1,0.1); 
			cr -> select_font_face ("Impact" , Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
			cr -> set_font_size ( xc/4 );
			cr -> move_to ( xc-(xc/8) , yc+(yc/2) );
			cr -> show_text (angleStr);
			
			// draw red lines out from the center of the window
			cr -> move_to (xc,yc);
			cr -> set_source_rgb (0.0 , 0.6078 , 0.9764);
			cr -> rotate ( angle  );
			// Original Position is UP
			cr -> rel_line_to ( 0, -((xc + yc)/2 - 20 ) );
			cr -> stroke();
			//cr -> save();
	
			//cr -> rel_line_to ( 10, -((xc + yc)/2 - 20 ) );
			//cr -> stroke();
			
			//cr -> restore();
			//cr -> save();
			//cr -> rel_line_to ( -10, -((xc + yc)/2 - 20 ) );
			//cr -> stroke();
			
		} 
		return true;
	} // KnobType = POSITIVE
	
	return true;
} // on_expose_event()


void RadialView::on_menu_file_popup_edit()
{
   std::cout << "The Edit menu item was selected." << std::endl;
}

void RadialView::on_menu_file_popup_generic()
{
   std::cout << "A popup menu item was selected." << std::endl;
}

bool RadialView::on_button_press_event(GdkEventButton* event)
{
	
	std::cout << "Event Type: " << event->type << ". Event Button: " << event->button << "." << std::endl;
	/*
	if( event->type == GDK_BUTTON_PRESS  ) // && event->button == 3
	{
		std::cout << "Event Type: " << event->type << ". Event Button: " << event->button << "." << std::endl;
		
		if(event->button == 3)
		{
			#ifdef DEBUG
			std::cout << "RadialView.cpp: Event RightClick on RadialView instance at " << this <<"." << std::endl;
			#endif
		}
		
		return true; //It's been handled.
	}
	else
	{
		std::cout << event -> type << std::endl;
	}
	*/
	return true;
}
