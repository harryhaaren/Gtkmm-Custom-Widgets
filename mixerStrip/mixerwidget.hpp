
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

#ifndef MIXERWIDGET
#define MIXERWIDGET

#include <gtkmm/drawingarea.h>


class MixerWidget : public Gtk::DrawingArea
{
public:
	MixerWidget();
	~MixerWidget();
	
	float getVolume();
	void  setVolume(float inVolume);

protected:
	float volume;
	
	void redraw();
	
	// Button & Expose event.
	bool on_button_press_event(GdkEventButton* event);
	bool on_expose_event(GdkEventExpose* event);
	bool onScrollEvent(GdkEventScroll *event);
};

#endif // MIXERWIDGET
