#include "dialogs.h"

// Static pointer to the main window
Gtk::Window* Dialogs::_win = 0;

// Remember the address of the main window object
void Dialogs::set_window(Gtk::Window& win) {
    _win = &win;
}

// Execute a dialog in context of the main window
int Dialogs::run_dialog(Gtk::Dialog* dialog) {
    if (_win) dialog->set_transient_for(*_win);
    return dialog->run();
}

// A message is like cout, simply displaying information to the user
void Dialogs::message(std::string msg, std::string title) {
    Gtk::MessageDialog *dialog = new Gtk::MessageDialog(title);
    if (_win) dialog->set_transient_for(*_win);
    dialog->set_secondary_text(msg, true);
    dialog->run();

    dialog->close();
    //while (Gtk::Main::events_pending())  Gtk::Main::iteration();

    delete dialog;
}

// A question is a message that allows the user to respond with a button
int Dialogs::question(std::string msg, std::string title,
             std::vector<std::string> buttons) {
    Gtk::Dialog *dialog = new Gtk::Dialog();
    if (_win) dialog->set_transient_for(*_win);
    dialog->set_title(title);

    Gtk::Label *label = new Gtk::Label(msg);
    label->set_use_markup(true);
    dialog->get_content_area()->pack_start(*label);
    label->show();

    for(int i=0; i<buttons.size(); ++i) dialog->add_button(buttons[i], i);

    int result = dialog->run();

    dialog->close();
    //while (Gtk::Main::events_pending())  Gtk::Main::iteration();

    delete label;
    delete dialog;

    return result;
}

// A request for a line of text input
std::string Dialogs::input(std::string msg, std::string title, std::string default_text, 
             std::string cancel_text) {
    Gtk::Dialog *dialog = new Gtk::Dialog();
    if (_win) dialog->set_transient_for(*_win);
    dialog->set_title(title);

    Gtk::Label *label = new Gtk::Label(msg);
    label->set_use_markup(true);
    dialog->get_content_area()->pack_start(*label);
    label->show();

    dialog->add_button("Cancel", 0);
    dialog->add_button("OK", 1);
    dialog->set_default_response(1);

    Gtk::Entry *entry = new Gtk::Entry{};
    entry->set_text(default_text);
    entry->set_max_length(50);
    entry->show();
    dialog->get_vbox()->pack_start(*entry);

    int result = dialog->run();
    std::string text = entry->get_text();

    dialog->close();
    //while (Gtk::Main::events_pending())  Gtk::Main::iteration();

    delete entry;
    delete label;
    delete dialog;

    if (result == 1)
        return text;
    else
        return cancel_text;
}

// Display an image from a disk file
void Dialogs::image(std::string filename, std::string title, std::string msg) {
    Gtk::Dialog *dialog = new Gtk::Dialog();
    if (_win) dialog->set_transient_for(*_win);
    dialog->set_title(title);

    Gtk::Label *label = new Gtk::Label(msg);
    label->set_use_markup(true);
    dialog->get_content_area()->pack_start(*label);
    label->show();

    dialog->add_button("Close", 0);
    dialog->set_default_response(0);

    Gtk::Image *image = new Gtk::Image{filename};
    image->show();
    dialog->get_vbox()->pack_start(*image);

    int result = dialog->run();

    dialog->close();
    //while (Gtk::Main::events_pending())  Gtk::Main::iteration();

    delete image;
    delete label;
    delete dialog;

    return;
}
