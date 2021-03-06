
#include <iostream>
#include <gtkmm.h>

#include "automationwidget.hpp"

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
	Gtk::VBox vbox;
	
	AutomationWidget array[5];
	
	for (int i = 0; i < 4; i++)
	{
		//array[i].setVolume();
		
		vbox.add( array[i] );
		
		// time is passed in as a float 0 = begin, 1 = end.
		array[i].update_time( (float)(i / 4.0));
		
	}
	
	// bind button to quit value of kit, by providing pointer to kit.
	button.signal_clicked().connect( sigc::bind(sigc::ptr_fun ( quit ), &kit));
	
	vbox.set_spacing(8);
	
	window.add(vbox);
	window.show_all();
	
	kit.run(window);
	
	return 0;
	
}
