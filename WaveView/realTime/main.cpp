
#include <iostream>
#include <cmath>
#include <gtkmm.h>

#include <jack/jack.h>

#include "waveview.hpp"

// nasty nasty globals
jack_port_t *input_port_L;
jack_client_t *client;
jack_default_audio_sample_t *in;

std::vector<float> buffer;

void quit(Gtk::Main *kit)
{
	kit->quit();
}

bool redraw(WaveView *waveview)
{
	waveview->redraw();
	
	//if (buffer.size() > 2048)
		//buffer.erase(buffer.begin() + 256);
	
	// stay connected
	return 1;
}

int process (jack_nframes_t nframes, void *arg)
{
	
	in = (jack_default_audio_sample_t *)	jack_port_get_buffer(input_port_L, nframes);
	
	float addedX = 0;
	// Left
	for (int i = 0; i < nframes; i++)
	{
		float x = in[i];
		addedX += x*x;
	}
	
	buffer.push_back( sqrt(addedX/nframes) );
	
	return 0;
}

int main(int argc , char *argv[])
{
	std::cout << "widget test: WaveView dialog" << std::endl;
	Gtk::Main kit (argc,argv);
	Gtk::Window window;
	Gtk::Button button("Quit");
	Gtk::HBox hbox;
	WaveView waveview;
	
	// setup JACK
	const char *client_name = "WaveView";
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;
	
	// open a client connection to the JACK server
	client = jack_client_open (client_name, options, &status, server_name);
	if (client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	
	input_port_L = jack_port_register (client, "input_L",
				 JACK_DEFAULT_AUDIO_TYPE,
				 JackPortIsInput, 0);
	
	jack_set_process_callback (client, process, 0);
	
	jack_activate(client);
	
	waveview.setSample( &buffer);
	
	//button.set_size(30,0);
	
	// bind button to quit value of kit, by providing pointer to kit.
	button.signal_clicked().connect( sigc::bind(sigc::ptr_fun ( quit ), &kit));
	
	Glib::signal_timeout().connect ( sigc::bind(sigc::ptr_fun( redraw ), &waveview) , 100 );
	
	hbox.add(waveview);
	hbox.add(button);
	
	window.add(hbox);
	window.show_all();
	kit.run(window);
	
	return 0;
	
}
