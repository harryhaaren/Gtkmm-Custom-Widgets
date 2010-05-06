#ifndef GTKMM_EXAMPLE_WaveView_H
#define GTKMM_EXAMPLE_WaveView_H

#include <gtkmm/drawingarea.h>


class WaveView : public Gtk::DrawingArea
{
public:
	WaveView();
	~WaveView();
	
	void redraw();
	void setSample(std::vector<float> *inSample);

protected:
	// the sample "loaded" is the real one. NOT a copy (ie: ByRef)
	std::vector<float> *sample;
	
	
	
	// Button & Expose event.
	bool on_button_press_event(GdkEventButton* event);
	bool on_expose_event(GdkEventExpose* event);
};

#endif // GTKMM_EXAMPLE_WaveView_H
