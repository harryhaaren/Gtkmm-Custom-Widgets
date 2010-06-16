
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

#ifndef SAMPLER_WIDGET
#define SAMPLER_WIDGET

#include <math.h>
#include <iostream>
#include <cstddef> // for string actions

//#include <gtkmm/eventbox.h>
//#include <cairomm/context.h>
	
#include <gtkmm.h>

class SamplerWidget : public Gtk::DrawingArea
{
	public:
		SamplerWidget();
		~SamplerWidget();
		
		void padOn();
		void padOff();
		bool playSample();
		bool loopSample();
		bool stopSample();
		bool fadeloopSample(int fade);
		bool setSamplerNumber(int inSamplerNum);
		bool setSampleName(std::string sample);
		bool setVolume(float volume);
		bool setPan(float pan);
	
	protected:
		float pan;
		float volume;
		int samplerNumber;
		std::string sampleName;
		
		bool padOnBool;
		bool loopSampleBool;
		
		//lo_address addr;
		Gtk::Allocation allocation;
		
		void redraw();
		bool onScrollEvent(GdkEventScroll *event);
		bool on_expose_event(GdkEventExpose* event);
		bool on_button_press_event	(GdkEventButton* event);
		bool on_button_release_event	(GdkEventButton* event);
		
};

#endif // SAMPLER_WIDGET

