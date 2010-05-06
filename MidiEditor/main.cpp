
#include <iostream>
#include <gtkmm.h>

#include <sndfile.hh>

#include "midieditor.hpp"

void quit(Gtk::Main *kit)
{
	kit->quit();
}

int main(int argc , char *argv[])
{
	std::cout << "widget test: WaveView dialog" << std::endl;
	Gtk::Main kit (argc,argv);
	Gtk::Window window;
	Gtk::Button button("Quit");
	Gtk::HBox hbox;
	MidiEditor waveview;
	
	// bind button to quit value of kit, by providing pointer to kit.
	button.signal_clicked().connect( sigc::bind(sigc::ptr_fun ( quit ), &kit));
	
	hbox.add(waveview);
	hbox.add(button);
	
	window.add(hbox);
	window.show_all();
	kit.run(window);
	
	return 0;
	
}
