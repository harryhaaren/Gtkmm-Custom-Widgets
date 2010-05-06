#ifndef MIDI_EDITOR
#define MIDI_EDITOR

#include <gtkmm/drawingarea.h>
#include <gtkmm/eventbox.h>
#include <vector>

typedef std::vector <unsigned char> MidiMessage;

class MidiEditor : public Gtk::DrawingArea
{
public:
	void showTrack();
	MidiEditor();
	virtual ~MidiEditor();
	void set_note_details( MidiMessage midiMessage );
	bool update();
	void set_bpm(int);


protected:
	int timer_callback();
	Gtk::Allocation allocation;
	int width , height;
	int pixelHeight;
	float playheadPosition;
	int noteVelocity;
	std::vector <MidiMessage> noteList;
	int noteStartTime;
	int noteEndPos;
	int noteOffWaiting;
	int timeElapsed;
	int totalTime;
	int noteStart;
	int updateRate;
	int noteEnd;
	int octave;
	int note;
	int bpm;

	//Override default signal handler:
	virtual bool on_expose_event			(GdkEventExpose* event);
	bool on_button_press_event	(GdkEventButton* event);
	
	// For Right-Click Actions
	void on_menu_file_popup_edit();
	void on_menu_file_popup_generic();
	
};

#endif // MIDI_EDITOR
