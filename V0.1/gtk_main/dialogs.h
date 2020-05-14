#ifndef _DIALOGS_H
#define _DIALOGS_H

#include <iostream>
#include <gtkmm.h>


class Dialogs {
  public:
    // Set the main window pointer for dialogs
    static void set_window(Gtk::Window& win);

    // Execute a dialog in context of the main window
    static int run_dialog(Gtk::Dialog* dialog);

    // A message is like cout, simply displaying information to the user
    static void message(std::string msg, std::string title = "Info");

    // A question is a message that allows the user to respond with a button
    static int question(std::string msg, std::string title = "Question",
                 std::vector<std::string> buttons = {"Cancel", "OK"});

    // A request for a line of text input
    static std::string input(std::string msg, std::string title = "Input", std::string default_text = "", 
                 std::string cancel_text = "CANCEL");

    // Display an image from a disk file
    static void image(std::string filename, std::string title = "Image", std::string msg = "");
  private:
    static Gtk::Window* _win;
};
#endif
