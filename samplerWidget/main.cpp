
#include <iostream>
#include <gtkmm.h>

#include "samplerwidget.hpp"

void quit(Gtk::Main *kit)
{
	kit->quit();
}

int main(int argc , char *argv[])
{
	std::cout << "widget test: Sampler widgets" << std::endl;
	Gtk::Main kit (argc,argv);
	Gtk::Window window;
	Gtk::Button button("Quit");
	Gtk::HBox hbox;
	
	SamplerWidget array[5];
	
	for (int i = 0; i < 4; i++)
	{
		hbox.add( array[i] );
	}
	
	array[1].padOn();
	array[2].loopSample();
	array[2].padOff();
	array[3].loopSample();
	
	// bind button to quit value of kit, by providing pointer to kit.
	button.signal_clicked().connect( sigc::bind(sigc::ptr_fun ( quit ), &kit));
	
	hbox.set_spacing(8);
	
	window.add(hbox);
	window.show_all();
	
	kit.run(window);
	
	return 0;
	
}
