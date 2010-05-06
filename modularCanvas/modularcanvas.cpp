#include "modularcanvas.hpp"


ModularCanvas::ModularCanvas()
{
	// Gives Exposures to the widget.
	add_events(Gdk::EXPOSURE_MASK);
	
	// set default widget size
	set_size_request(400,350);
	
	CanvasObject x;
	
	x.setInfo(10,10,50,50);
	objectVector.push_back(x);
	
	x.setInfo(80,80,250,250);
	objectVector.push_back(x);
}

ModularCanvas::~ModularCanvas()
{
}


bool ModularCanvas::on_expose_event(GdkEventExpose* event)
{
	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = get_window();
	
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	std::cout << "width: " << width << "\t height:" << height << std::endl;
	
	// coordinates for the center of the window
	int xc, yc;
	xc = width / 2;
	yc = height / 2;
	
	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
	
	std::cout << cr << std::endl;
	
	// clip to the area indicated by the expose event so that we only redraw
	// the portion of the window that needs to be redrawn
	cr->rectangle(event->area.x, event->area.y,event->area.width, event->area.height);
	cr->clip();
	
	for (unsigned int i = 0; i < objectVector.size(); i++)
	{
		cr-> rectangle(objectVector[i].getX(),objectVector[i].getY(),objectVector[i].getWidth(),objectVector[i].getHeight());
	}
	cr->set_source_rgb(0.3,0.3,0.3);
	cr->stroke();
	
	return true;
} // on_expose_event()

void ModularCanvas::redraw()
{
	// force our program to redraw the entire widget.
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),get_allocation().get_height());
		win->invalidate_rect(r, false);
	}
}
