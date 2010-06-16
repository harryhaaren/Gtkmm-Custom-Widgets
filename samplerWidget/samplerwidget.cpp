
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

#include "samplerwidget.hpp"

SamplerWidget::SamplerWidget()
{
	//std::cout << "SamplerWidget()" << std::endl;
	
	set_size_request(200, 150);
	
	// set initial volume
	volume	= 0.8;
	
	// initial pan: -1.0 = left 1.0 = right  0.0 = center
	pan		= 0.0;
	
	loopSampleBool = false;
	padOff();
	
	// let this widget recieve these events
	add_events( Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |Gdk::SCROLL_MASK);
	
	// Handle the signals of the events by callback funtcions
	signal_scroll_event().connect( sigc::mem_fun( *this, &SamplerWidget::onScrollEvent ) );
	signal_button_press_event().connect(sigc::mem_fun(*this, &SamplerWidget::on_button_press_event) );
	signal_button_release_event().connect(sigc::mem_fun(*this, &SamplerWidget::on_button_release_event) );
}

SamplerWidget::~SamplerWidget()
{
	//std::cout << "~SamplerWidget()" << std::endl;
}

bool SamplerWidget::setSampleName(std::string sample)
{
	sampleName = sample;
	
	redraw();
	
	return true;
}

bool SamplerWidget::playSample()
{
	loopSampleBool = false;
	padOn();
	redraw();
	return true;
}

bool SamplerWidget::loopSample()
{
	loopSampleBool = true;
	padOn();
	
	// args are SamplerNum,numLoops,fade
	//lo_send(addr, "/luppp/sampler/play","iii", samplerNumber,-1,0);
	redraw();
	return true;
}

bool SamplerWidget::fadeloopSample(int fade = 350)
{
	padOn();
	loopSampleBool = true;
	redraw();
	// args are SamplerNum,numLoops,fade
	
	return true;
}

bool SamplerWidget::stopSample()
{
	padOff();
	loopSampleBool = false;
	
	redraw();
	return true;
}

bool SamplerWidget::setVolume(float inVolume)
{
	std::cout << "Got Volume Change MEssage in samplerwidget.cpp" << std::endl;
	volume = inVolume;
	redraw();
	
	return true;
}

bool SamplerWidget::setPan(float inPan)
{
	pan = inPan;
	//lo_send(addr, "/luppp/sampler/pan","if", samplerNumber,pan);
	redraw();
	return true;
}

bool SamplerWidget::setSamplerNumber(int inSamplerNum)
{
	samplerNumber = inSamplerNum;
	return 0;
}

void SamplerWidget::redraw()
{
	// force redraw of the entire widget.
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
								get_allocation().get_height());
		win->invalidate_rect(r, false);
	}
	return;
}

void SamplerWidget::padOn()
{
	padOnBool = true;
	redraw();
}
void SamplerWidget::padOff()
{
	padOnBool = false;
	redraw();
}

bool SamplerWidget::onScrollEvent(GdkEventScroll *event)
{
	if (event->direction != 1) 
	{
		// Up volume
		volume += 0.1;
		if (volume > 1.0) {volume = 1.0000;}
		//lo_send(addr, "/luppp/sampler/volume","if", samplerNumber,volume);
	}
	else
	{
		// Down volume
		volume = volume - 0.1;
		if (volume < 0.0) {volume = 0.00;}
		//lo_send(addr, "/luppp/sampler/volume","if", samplerNumber,volume);
	}
	redraw();
}

bool SamplerWidget::on_button_press_event(GdkEventButton* event)
{
	//std::cout << "Press  Event Type: " << event->type << ". Event Button: " << event->button << "." << std::endl;
	
	if( event->type == GDK_BUTTON_PRESS && event->button == 1 ) // left click=play
	{
		//lo_send(addr, "/luppp/sampler/play","i", samplerNumber); // play
		padOn();
		redraw();
		return true;
	}
	if( event->type == GDK_BUTTON_PRESS && event->button == 2 ) // change me
	{
		std::string loadThisSample;
		
		
		// Hack fix: fileChooser wasnt linking: creating one here instead.
		Gtk::FileChooserDialog dialog( "Choose a Sample to load" , Gtk::FILE_CHOOSER_ACTION_OPEN);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button("Select", Gtk::RESPONSE_OK);
		
		int result = dialog.run();
		
		switch (result)
		{
			case Gtk::RESPONSE_OK:
			{
				//std::cout << "FileChooser: Response Accept" << std::endl;
				loadThisSample = dialog.get_filename();
				break;
			}
			default:
			{
				//std::cout << "FileChooser: Response Cancel" << std::endl;
				loadThisSample = "";
				break;
			}
		}
		// end hack fix
		
		// Original code
		//FileChooser fileChooser
		//loadThisSample = fileChooser.show("Choose a Sample to load!",Gtk::FILE_CHOOSER_ACTION_OPEN);
		
		
		
		// Send OSC load command
		if (loadThisSample != "")
		{
			//lo_send(addr, "/luppp/sampler/load","is", samplerNumber,loadThisSample.c_str());
			
			// Now cut the path & filename to filename, and set the samples "name"
			loadThisSample = loadThisSample.substr(loadThisSample.rfind("/")+1);
			std::size_t pos = loadThisSample.rfind(".");
			
			// For checking string lenghts
			std::cout << "POS: " << pos << std::endl;
			
			if (loadThisSample.size() < 10)
				setSampleName( loadThisSample.substr( 0,  loadThisSample.size() - pos  ) );
			else
				setSampleName( loadThisSample.substr( 0,  9 ) ); // max lenght 9 chars
		}
		redraw();
		return true;
		
	}
	else if ( event->type == GDK_BUTTON_PRESS && event->button == 3 ) //  Change mode
	{
		if( !loopSampleBool )
		{
			//lo_send(addr, "/luppp/sampler/play","iii", samplerNumber,-1,0); // loop inf
			loopSample();
		}
		else
		{
			stopSample();
			//lo_send(addr, "/luppp/sampler/stop","i", samplerNumber); //stop
		}
		redraw();
		return true;
	}
	redraw();
}

bool SamplerWidget::on_button_release_event(GdkEventButton* event)
{
	if( event->type == GDK_BUTTON_RELEASE && event->button == 3)
	{
		return true; // right click = loop. Leave Orange on!
	}
	else
	{
		padOff();
	}
	redraw();
	return false;
}

bool SamplerWidget::on_expose_event(GdkEventExpose* event)
{
	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = get_window();
	
	if(window)    // Only run if Window does exist
	{
		allocation = get_allocation();
		int width = allocation.get_width();
		int height = allocation.get_height();
		
		//std::cout << "Width: " << width << std::endl;
		//std::cout << "Height: " << height << std::endl;
		//std::cout << "Height / 12: " << height /12.0 << std::endl;
		
		// coordinates for the center of the window
		int xc, yc;
		xc = width / 2;
		yc = height / 2;
		
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		
		// clip to the area indicated by the expose event so that we only redraw
		// the portion of the window that needs to be redrawn
		//cr -> set_source_rgba(0 , 0, 0 , 1);
		cr->rectangle(event->area.x, event->area.y,
		        event->area.width, event->area.height);
		//cr->rectangle(0,0,event->area.x,event->area.y);
		//cr->stroke_preserve();
		cr->clip();
		
		cr -> set_line_join (Cairo::LINE_JOIN_ROUND);
		
		cr -> move_to( 7, 7);
		cr -> line_to( width-7, 7);
		cr -> line_to( width-7, height-7);
		cr -> line_to( 7, height-7);
		cr -> close_path();
		
		// Draw outline shape
		cr -> set_source_rgb (0.1,0.1,0.1);
		cr -> fill_preserve();
		
		if (!padOnBool)
		{
			cr->set_source_rgba( 0.6, 0.6, 0.6, 0.5);
			cr->set_line_width ( 10.0);
			cr->stroke();
		}
		else
		{
			cr->set_source_rgba( 1.0,0.4,0.0, 0.8);
			cr->set_line_width ( 10.4);
			cr->stroke();
		}
		
		/* Pan functionality Not yet implemented in sampler.
		
		// Draw Pan line: pixel pan = width * pan
		cr -> set_line_cap (Cairo::LINE_CAP_ROUND);
		cr -> set_line_width ( 4.8);
		cr -> set_source_rgba( 1.0,0.8,0.8,0.8);
		cr -> move_to ( width/2 + ((width*pan)/2) , height - (height*volume) + 20);
		cr -> line_to ( width/2 + ((width*pan)/2) , height - (height*volume) - 20);
		cr -> stroke();
		*/
		
		// Draw Volume line: pixel vol = height * volume
		cr -> set_line_width ( 5.1);
		cr -> set_source_rgb( 0.2,0.2,0.2);
		cr -> move_to ( 10      , (height - ((height-20)*volume))-8 );
		cr -> line_to ( width-10, (height - ((height-20)*volume))-8 );
		cr -> line_to ( width-10, height-10);
		cr -> line_to ( 10, height-10);
		cr -> close_path();
		cr -> fill();
		
		// Draw Mode: Loop/Hit
		
		if ( !loopSampleBool)
		{
			cr->set_line_width ( 2.2);
			cr->set_source_rgb(0.2,0.2,0.2);
			cr->move_to(width/5,(height/5)*2);
			cr->line_to((width/5)*3,(height/5)*2);
			cr->line_to((width/5)*3,(height/5)*1);
			cr->line_to((width/5)*4,height/2);		// point of arrow
			cr->line_to((width/5)*3,(height/5)*4);
			cr->line_to((width/5)*3,(height/5)*3);
			cr->line_to(width/5,(height/5)*3);
			cr->close_path();
			cr->fill_preserve();
			
			cr->set_source_rgb(0.0,0.9,0.0);
			cr->stroke();
		}
		else // Loop Sample
		{
			cr -> set_source_rgb(0.2,0.2,0.2);
			cr -> arc_negative	(width/2, height/2, ((height  + width)/8)-5 , 6.0, 0.6); 	// inner curve
			cr -> arc			(width/2, height/2, ((height  + width)/6)-5 , 0.5, 6.1); 	// outer curve
			
			cr -> line_to		((width-width/3.5) + width/8  ,	height/2 - height/18 ); 	// right point of arrow
			cr -> line_to		( width-width/3.5,				height/2 + height/18 ); 	// point of arrow
			cr -> line_to		((width-width/3.5) - width/8  ,	height/2 - height/18 ); 	// left point of arrow
			cr -> close_path();
			cr -> fill_preserve();
			cr -> set_line_width(1.9);
			cr -> set_source_rgb(0.0,0.9,0.0);
			cr -> stroke();
		}
		
		// Impose orange volume line (over SampleLoop/Play)
		cr -> move_to ( 10      , (height - ((height-20)*volume))-8 );
		cr -> line_to ( width-10, (height - ((height-20)*volume))-8 );
		cr -> set_source_rgb(1.0,0.4,0.0);
		cr -> stroke();
		
		// Draw Text in center. Audio Clip name.
		cr -> set_source_rgb( 1.0,0.4,0.0);
		cr -> select_font_face ("Impact" , Cairo::FONT_SLANT_NORMAL,Cairo::FONT_WEIGHT_NORMAL);
		cr -> set_font_size ( 3 + xc/4 );
		Cairo::TextExtents extents;
		std::transform(sampleName.begin(), sampleName.end(), sampleName.begin(), toupper);
		cr -> get_text_extents(sampleName, extents);
		cr -> move_to ( xc - extents.width/2 , height/2+height/18);
		cr -> show_text (sampleName); // sampleName defined in hpp as std::string
	} 
	return true;
}
