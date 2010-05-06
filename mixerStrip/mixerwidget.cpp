
/*	This file is part of Luppp.
	Author: Harry van Haaren
	E-mail: harryhaaren@gmail.com
	Copyright (C) 2010 Harry van Haaren

	Luppp is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Luppp is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Luppp.  If not, see <http://www.gnu.org/licenses/>. */

#include "mixerwidget.hpp"

#include <iostream>
#include <cairomm/context.h>
#include <sstream>

MixerWidget::MixerWidget()
{
	volume = 0.75;
	
	// Gives Exposure & Button presses to the widget.
	add_events(Gdk::EXPOSURE_MASK);
	add_events(Gdk::BUTTON_PRESS_MASK );
	add_events(Gdk::SCROLL_MASK);
	
	signal_scroll_event().connect( sigc::mem_fun( *this, &MixerWidget::onScrollEvent ) );
	
	// set default widget size
	set_size_request(50,250);
}

MixerWidget::~MixerWidget()
{
}

float MixerWidget::getVolume()
{
	return volume;
}

void MixerWidget::setVolume(float inVolume)
{
	volume = inVolume;
}

bool MixerWidget::on_expose_event(GdkEventExpose* event)
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
			cr->clip();
			
			cr->set_line_join(Cairo::LINE_JOIN_ROUND);
			
			// vertical line
			cr->set_line_cap(Cairo::LINE_CAP_ROUND);
			cr->set_line_width(5);
			cr->set_source_rgb (0.1,0.1,0.1);
			cr->move_to(width/2			,height-height*9/10); // top
			cr->line_to(width/2			,height-height*1/10); // bottom
			cr->line_to(width-width*1/10,height-height*9/10); // right
			cr->close_path();
			cr->stroke_preserve();
			cr->set_source_rgba( 0.6, 0.6, 0.6, 0.5);
			cr->set_line_width(3);
			cr->stroke();
			
			// Fader position
			cr->set_source_rgba( 1.0,0.4,0.0, 0.8);
			int faderPos	= (height-(height*8/10)*volume)-height*1/10;
			int left		= width*1/10;
			
			cr->move_to(width/2,faderPos);
			cr->line_to(left,faderPos+height*1/18);
			cr->line_to(left,faderPos-height*1/18);
			cr->close_path();
			cr->stroke();
		}
	return true;
} // on_expose_event()

void MixerWidget::redraw()
{
	// force our program to redraw the entire widget.
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),get_allocation().get_height());
		win->invalidate_rect(r, false);
	}
}

bool MixerWidget::onScrollEvent(GdkEventScroll *event)
{
	if (event->direction != 1) 
	{
		// UP?
		volume = volume + 0.01;
		if (volume > 1.0) { volume = 1.0; }
	}
	else
	{
		volume = volume - 0.01;
		if (volume < 0.0) { volume = 0.0; }
	}
	
	redraw();
	return 0;
	
}

bool MixerWidget::on_button_press_event(GdkEventButton* event)
{
	/*
	sample->clear();
	
	if( event->type == GDK_BUTTON_PRESS  ) // && event->button == 3
	{
		sample->push_back(-1);
		return true; //It's been handled.
	}
	else
	{
		sample->push_back(1);
	}
	
	redraw();
	*/
	return true;
}
