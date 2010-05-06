#ifndef JACKLABEL
#define JACKLABEL

#include <gtkmm/drawingarea.h>

class JackLabel : public Gtk::DrawingArea
{
public:
	JackLabel();
	~JackLabel();
	
	void setBBT(long inBar,int inBeat,int inTick );
	void setXruns();

protected:
	float volume;
	
	void redraw();
	
	int bar,beat,tick;
	
	// Button & Expose event.
	bool on_button_press_event(GdkEventButton* event);
	bool on_expose_event(GdkEventExpose* event);
	bool onScrollEvent(GdkEventScroll *event);
};

#endif // JACKLABEL
