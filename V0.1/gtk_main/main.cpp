#include <gtkmm.h>

#include "mainwin.h"

int main(int argc, char** argv)
{
	auto app = Gtk::Application::create(argc, argv, "com.aeromavericks");
	
	//Instance a window
	Mainwin win;
	
	//Give control to GTK
	return app->run(win);

}
