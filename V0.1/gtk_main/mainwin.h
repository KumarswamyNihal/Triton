#ifndef MAINWIN_H
#define MAINWIN_H

#include <gtkmm.h>
#include <stdlib.h>
#include "controller.h"

class Mainwin : public Gtk::Window
{
	public:
		Mainwin();
		virtual ~Mainwin();
	protected:
		void on_reset_p0_click();
		void on_reset_p1_click();
		void on_reset_p2_click();
		void on_reset_click();
		void on_openport_click();
		void on_closeport_click();
		void on_close_click();
		void on_open_valve_click();
		void on_close_valve_click();
		void on_open_1s_click();
		void on_open_5s_click();
		void on_open_10s_click();
		void on_open_15s_click();
		void on_about_click();
		void on_help_click();
		void on_save_click();
		bool update_display();

		class ModelColumns : public Gtk::TreeModel::ColumnRecord {
			public:
				ModelColumns() {
					add(m_id);
					add(m_name);
					add(m_value);
					add(m_state);
					add(m_max);
					add(m_min);
				}

			Gtk::TreeModelColumn<unsigned int> m_id;
			Gtk::TreeModelColumn<Glib::ustring> m_name;
			Gtk::TreeModelColumn<float> m_value;
			Gtk::TreeModelColumn<std::string> m_state;
			Gtk::TreeModelColumn<float> m_max;
			Gtk::TreeModelColumn<float> m_min;
		};

		ModelColumns _columns;

		Glib::RefPtr<Gtk::ListStore> _tree_model;

		Gtk::TreeView _tree_view;
		Gtk::ScrolledWindow _swindow;

	private:
		void close_after(int sec);
		Controller _controller;
};
#endif
