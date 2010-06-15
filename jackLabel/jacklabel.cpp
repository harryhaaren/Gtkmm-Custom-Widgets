
#include "jacklabel.hpp"


JackLabel::JackLabel()
{
	// Gives Exposures to the widget.
	add_events(Gdk::EXPOSURE_MASK);
	
	// set default widget size
	set_size_request(120,80);
	
	xruns = false;
	
	bar = 20;
	beat = 2;
	tick = 150;
}

JackLabel::~JackLabel()
{
}

void JackLabel::setBBT(long inBar,int inBeat,int inTick )
{
	std::cout << "JackLabel::setBBT("<<inBar<<" "<<inBeat<<" "<<inTick<<");"<<std::endl;
	bar  = inBar;
	beat = inBeat;
	tick = inTick;
}

void JackLabel::setXruns(bool inXruns)
{
	std::cout << "XRUNS!!!!" << std::endl;
	xruns = inXruns;
}

bool JackLabel::on_expose_event(GdkEventExpose* event)
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
			
			// clip to the area indicated by the expose event so that we only redraw
			// the portion of the window that needs to be redrawn
			cr->rectangle(event->area.x, event->area.y,event->area.width, event->area.height);
			
			if (xruns)
				cr->set_source_rgb(0.5,0.0,0.0);	// red
			else
				cr->set_source_rgb(0.1,0.1,0.1);	// black
			
			
			// fill the area, and clip to that area
			cr->fill_preserve();
			cr->clip();
			
			cr -> select_font_face ("Impact" , Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
			cr -> set_font_size ( 20 );
			cr -> set_source_rgb ( 1.0,0.4,0.0);
			cr -> move_to ( 8,height/3 );
			cr -> show_text ("Bpm: 120");
			cr -> move_to ( 8,height*9.5/12 );
			cr -> show_text ("4:3:100");
		}
	return true;
} // on_expose_event()

void JackLabel::redraw()
{
	// force our program to redraw the entire widget.
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),get_allocation().get_height());
		win->invalidate_rect(r, false);
	}
}
