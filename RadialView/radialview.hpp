#ifndef GTKMM_EXAMPLE_RADIALVIEW_H
#define GTKMM_EXAMPLE_RADIALVIEW_H

#include <gtkmm/drawingarea.h>

enum KnobType 
{
	POSITIVE = 0,
	BIPOLAR = 1,
	NEGATIVE = 2,
	ENDLESS = 3,
};

class RadialView : public Gtk::DrawingArea
{
public:
  void set_angle(float angleToSet);
  bool update();
  RadialView();
  virtual ~RadialView();

protected:
  int bpm;
  KnobType knobType;
  int totalTime;
  float angle;
  std::string angleStr;
  
  //Override default signal handler:
  void on_menu_file_popup_edit();
  void on_menu_file_popup_generic();
  
  // Button & Expose event.
  bool on_button_press_event(GdkEventButton* event);
  bool on_expose_event(GdkEventExpose* event);
};

#endif // GTKMM_EXAMPLE_RADIALVIEW_H
