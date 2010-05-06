
#include "midieditor.hpp"
#include <iostream>
#include <cairomm/context.h>
#include <vector>

MidiEditor::MidiEditor()
{
	//std::cout << "Enterin MidiEditor contructor" << std::endl;
	add_events(Gdk::EXPOSURE_MASK);
	
	// Gives Button presses to the widget.
	add_events(Gdk::BUTTON_PRESS_MASK);
	
	signal_button_press_event().connect(sigc::mem_fun(*this, &MidiEditor::on_button_press_event) );
	
	// Get size of Widget
	allocation = get_allocation();
	width = allocation.get_width();
	height = allocation.get_height();
	
	bpm = 120;
	note = 0;
	octave = 0;
	timeElapsed = 0;
	totalTime = (( 60.0 / bpm) * 8) * 1000 ;  // *1000 from seconds to milliSeconds
	playheadPosition = 0;
	updateRate = 20;
	
	// Vector to store all notes.
	// noteList is this vector. It is initialized to 0.
	
	noteStartTime = 0;
	noteEndPos = 0;
	noteOffWaiting = 1;
	
	set_size_request(250,200); // Minimum size for widget in pixels
	
	// Set Playhead Callback.               
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &MidiEditor::timer_callback), updateRate);
	
	//std::cout << "MidiEditor::Constructor ok!" << std::endl;
}

int MidiEditor::timer_callback()
{
	playheadPosition = ( float(timeElapsed) / float(totalTime) ) * (width/1.0); 
	
	if (timeElapsed > totalTime)
	{
		timeElapsed = 0 ;
		playheadPosition = ( float(timeElapsed) / float(totalTime) ) * (width/1.0); 
		/*std::cout << "timeElapsed: " << timeElapsed << "msecs." << std::endl;
		std::cout << "Total Play time: " << totalTime << "msecs." << std::endl;
		std::cout << "playheadPosition: " << playheadPosition << std::endl;
		*/
	}
	
	update();
	
	timeElapsed = timeElapsed + updateRate;
	
	return true;
}

MidiEditor::~MidiEditor()
{
}


void MidiEditor::set_bpm(int inbpm)
{
	bpm = inbpm;
}

bool MidiEditor::update()
{
	// force our program to redraw the entire widget.
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
	    Gdk::Rectangle r(0, 0, get_allocation().get_width(),
	            get_allocation().get_height());
	    win->invalidate_rect(r, false);
	}
	return 0;
}

void MidiEditor::set_note_details( MidiMessage midiMessage)
{
}

bool MidiEditor::on_expose_event(GdkEventExpose* event)
{
	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = get_window();
	
	if(window)    // Only run if Window does exist
	{
		allocation = get_allocation();
		width = allocation.get_width();
		height = allocation.get_height();
		
		//std::cout << "Width: " << width << std::endl;
		//std::cout << "Height: " << height << std::endl;
		//std::cout << "Height / 12: " << height /12.0 << std::endl;
		
		// coordinates for the center of the window
		int xc, yc;
		xc = width / 2;
		yc = height / 2;
		
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		
		// set line Width
		cr -> set_line_width(2*width);
		//cr -> set_line_join (CAIRO::LINE_JOIN_ROUND);
		
		
		// clip to the area indicated by the expose event so that we only redraw
		// the portion of the window that needs to be redrawn
		cr->rectangle(event->area.x, event->area.y,event->area.width, event->area.height);
		cr->clip();
		
		
		// Draw grid in grey.
		float x= 0;
		
		cr -> set_line_width(1.4);
		cr -> set_source_rgba(0.2 , 0.2 , 0.2 , 1);
		
		// Top line & Left line.. werent coverd by loop
		cr -> move_to (0,0);
		cr -> line_to (width ,0);
		cr -> move_to (0,0);
		cr -> line_to (0,height);
		
		int noteNumber = 0;
		while (x < height)
		{
			x = x + height/12.0;
			
			cr -> set_source_rgba(0.2 , 0.2 , 0.2 , 1);
			cr -> move_to (0,x);
			cr -> line_to (width ,x);
			cr -> stroke();
			noteNumber++;
		}
		
		x = 0;
		while (x < width)
		{
			x = x + width / 8.0;
			cr -> move_to (x, 0);
			cr -> line_to (x , height);
			cr -> stroke();
		}
		x=0;
		// Grid Done...
		
		// Default PixelHeight
		pixelHeight = height - (height/12.0) + (height / 24.0);
		
		for (int i = 0; note >= 13; i++ )
		{
			note = note - 12;
			octave = i; // octave is used to change colour
		}
		
		if ( note > 0)
		{
			//	Note here is the amount of semitones above Mid C.
			pixelHeight = height - (height/12.0)*(note) + (height / 24.0);
		}
		else
		{
			//std::cout << "In ELSE Clause: " << std::endl;
			pixelHeight = height - (height/12.0) + (height / 24.0);
		}
		
		//std::cout << "Note , Note Velocity : " << note << "   " << noteVelocity <<std::endl;
		//std::cout << "PixelHeight before drawing: " << pixelHeight << std::endl;
		
		cr -> set_line_width(10.2 *width/200);
		cr -> set_line_cap (Cairo::LINE_CAP_ROUND);
		
		//  Curve (x1, y1, x2, y2, x3, y3) 
		//cr -> curve_to (0, (pixelHeight + height/6),100 , pixelHeight-100, width,pixelHeight);
		//cr -> move_to (0 , pixelHeight);
		//cr -> line_to (width , pixelHeight );
		
		cr -> set_source_rgb (0.1,0.1,0.1);
		cr -> move_to (noteStartTime , pixelHeight );
		if (noteOffWaiting)
		{
			cr -> line_to (playheadPosition    , pixelHeight );
		}
		else
		{
			cr -> line_to (noteEndPos    , pixelHeight );
		}
		cr -> stroke_preserve();
		
		cr -> set_source_rgba(1.0 , 1.0 , 1.0 , 1) ; // default is WHITE
		if (octave  == 1) { cr -> set_source_rgba(1.0 , 0.0 , 0.0 , 1) ; }
		if (octave  == 2) { cr -> set_source_rgba(0.0 , 1.0 , 0.0 , 1) ; }
		if (octave  == 3) { cr -> set_source_rgba(0.0 , 0.0 , 1.0 , 1) ; }
		if (octave  == 4) { cr -> set_source_rgba(1.0 , 1.0 , 0.0 , 1) ; }
		if (octave  == 5) { cr -> set_source_rgba(0.0 , 1.0 , 1.0 , 1) ; }
		if (octave  == 6) { cr -> set_source_rgba(0.5 , 0.5 , 1.0 , 1) ; }
		if (octave  == 7) { cr -> set_source_rgba(1.0 , 0.0 , 1.0 , 1) ; }
		if (octave  == 8) { cr -> set_source_rgba(0.0 , 0.0, 0.0 , 1)  ; }
		
		cr -> set_line_width(5.1 *width/200);
		cr -> move_to (noteStartTime, pixelHeight);
		if (noteOffWaiting)
		{
			//cr -> line_to (playheadPosition    , pixelHeight );
		}
		else
		{
			cr -> line_to (noteEndPos    , pixelHeight );
		}
		cr -> stroke();
		
		// Draw Playhead
		cr -> set_line_width (2.5);
		cr -> set_line_cap (Cairo::LINE_CAP_SQUARE);
		cr -> set_source_rgb ( 1.0 , 0.0 , 0.0);
		cr -> move_to (playheadPosition , 0);
		cr -> line_to (playheadPosition , height);
		cr -> stroke();
		
		//std::cout << "Updated MidiEditor." << std::endl;
	} 
	return true;
}

void MidiEditor::on_menu_file_popup_edit()
{
   std::cout << "The Edit menu item was selected." << std::endl;
}

void MidiEditor::on_menu_file_popup_generic()
{
   std::cout << "A popup menu item was selected." << std::endl;
}

bool MidiEditor::on_button_press_event(GdkEventButton* event)
{
  if( event->type == GDK_BUTTON_PRESS  ) // && event->button == 3
  {
    if(true)
      //m_pMenuPopup->popup(event->button, event->time);
      std::cout << "Event Type: " << event->type << ". Event Button: " << event->button << "." << std::endl;
      
    return true; //It's been handled.
  }
  else
    return false;
}
