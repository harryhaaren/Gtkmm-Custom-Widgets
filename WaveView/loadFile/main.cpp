
#include <iostream>
#include <gtkmm.h>

#include <sndfile.hh>

#include "waveview.hpp"

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
	WaveView waveview;
	
	// predefined lenght
	// const int size = 44100*0.42;
	
	std::vector<float> sampleBuffer;
	SndfileHandle infile("foo.flac", SFM_READ,SF_FORMAT_FLAC | SF_FORMAT_FLOAT, 1, 44100);
	
	const int size = infile.frames();
	
	std::cout << "Size:" << size << "SampRate:" << infile.samplerate() << "Frames:" << infile.frames() << std::endl;
	
	// RESIZE your vector to fit entire soundFile into buffer!
	sampleBuffer.resize(size);
	
	// loads entire file into buffer.
	infile.read(&sampleBuffer[0],size);
	waveview.setSample( &sampleBuffer);
	
	//button.set_size(30,0);
	
	// bind button to quit value of kit, by providing pointer to kit.
	button.signal_clicked().connect( sigc::bind(sigc::ptr_fun ( quit ), &kit));
	
	hbox.add(waveview);
	hbox.add(button);
	
	window.add(hbox);
	window.show_all();
	kit.run(window);
	
	return 0;
	
}
