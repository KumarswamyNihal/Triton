#include <stdlib.h>
#include "mainwin.h"
#include "dialogs.h"

Mainwin::Mainwin() : _controller{Controller{}} {
	
	//Provide a window for dialogs
	Dialogs::set_window(*this);
	
	////////////////
	//GUI SETUP
	////////////////
	
	set_default_size(800, 600);

	Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	add(*vbox);

	///////////////
	//MENU
	//////////////
	Gtk::MenuBar *menubar = Gtk::manage(new Gtk::MenuBar());
	vbox->pack_start(*menubar, Gtk::PACK_SHRINK, 0);


	//FILE MENU
	Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
	menubar->append(*menuitem_file);
	Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
	menuitem_file->set_submenu(*filemenu);
	
	//Open Port
	Gtk::MenuItem *menuitem_openport = Gtk::manage(new Gtk::MenuItem("_Open Port", true));
	menuitem_openport->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_openport_click));
	filemenu->append(*menuitem_openport);

	//Close Port
	Gtk::MenuItem *menuitem_closeport = Gtk::manage(new Gtk::MenuItem("_Close Port", true));
	menuitem_closeport->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_closeport_click));
	filemenu->append(*menuitem_closeport);
	
	//Save
	Gtk::MenuItem *menuitem_save = Gtk::manage(new Gtk::MenuItem("_Save", true));
	menuitem_save->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_save_click));
	filemenu->append(*menuitem_save);
	
	//Close
	Gtk::MenuItem *menuitem_close = Gtk::manage(new Gtk::MenuItem("_Close", true));
	menuitem_close->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_close_click));
	filemenu->append(*menuitem_close);
	

	//PRESSURE MENU	
	Gtk::MenuItem *menuitem_pressure = Gtk::manage(new Gtk::MenuItem("_Pressure", true));
	menubar->append(*menuitem_pressure);
	Gtk::Menu *pressuremenu = Gtk::manage(new Gtk::Menu());
	menuitem_pressure->set_submenu(*pressuremenu);

	//Reset All 
	Gtk::MenuItem *menuitem_reset = Gtk::manage(new Gtk::MenuItem("_Reset Max", true));
	menuitem_reset->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_reset_click));
	pressuremenu->append(*menuitem_reset);
	
	//Reset P0 Max
	Gtk::MenuItem *menuitem_resetp0 = Gtk::manage(new Gtk::MenuItem("_Reset P0 Max", true));
	menuitem_resetp0->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_reset_p0_click));
	pressuremenu->append(*menuitem_resetp0);

	//Reset P1 Max
	Gtk::MenuItem *menuitem_resetp1 = Gtk::manage(new Gtk::MenuItem("_Reset P1 Max", true));
	menuitem_resetp1->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_reset_p1_click));
	pressuremenu->append(*menuitem_resetp1);

	//Reset P2 Max
	Gtk::MenuItem *menuitem_resetp2 = Gtk::manage(new Gtk::MenuItem("_Reset P2 Max", true));
	menuitem_resetp2->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_reset_p2_click));
	pressuremenu->append(*menuitem_resetp2);

	
	//Valve 
	Gtk::MenuItem *menuitem_valve = Gtk::manage(new Gtk::MenuItem("_Valve", true));
	menubar->append(*menuitem_valve);
	Gtk::Menu *valvemenu = Gtk::manage(new Gtk::Menu());
	menuitem_valve->set_submenu(*valvemenu);

	//Open Valve
	Gtk::MenuItem *menuitem_openvalve = Gtk::manage(new Gtk::MenuItem("_Open", true));
	valvemenu->append(*menuitem_openvalve);
	Gtk::Menu *openmenu = Gtk::manage(new Gtk::Menu());
	menuitem_openvalve->set_submenu(*openmenu);

	//1s
	Gtk::MenuItem *menuitem_1s = Gtk::manage(new Gtk::MenuItem("_1s", true));
	menuitem_1s->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_open_1s_click));
	openmenu->append(*menuitem_1s);

	//5s
	Gtk::MenuItem *menuitem_5s = Gtk::manage(new Gtk::MenuItem("_5s", true));
	menuitem_5s->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_open_5s_click));
	openmenu->append(*menuitem_5s);
	

	//10s
	Gtk::MenuItem *menuitem_10s = Gtk::manage(new Gtk::MenuItem("_10s", true));
	menuitem_10s->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_open_10s_click));
	openmenu->append(*menuitem_10s);
	
	//15s
	Gtk::MenuItem *menuitem_15s = Gtk::manage(new Gtk::MenuItem("_15s", true));
	menuitem_15s->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_open_15s_click));
	openmenu->append(*menuitem_15s);
	
	
	//Open
	Gtk::MenuItem *menuitem_op = Gtk::manage(new Gtk::MenuItem("_Open", true));
	menuitem_op->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_open_valve_click));
	openmenu->append(*menuitem_op);

	//Close Valve
	Gtk::MenuItem *menuitem_closevalve = Gtk::manage(new Gtk::MenuItem("_Close", true));
	menuitem_closevalve->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_close_valve_click));
	valvemenu->append(*menuitem_closevalve);
	
	//Help Menu
	Gtk::MenuItem *menuitem_help = Gtk::manage(new Gtk::MenuItem("_Help", true));
	menubar->append(*menuitem_help);
	Gtk::Menu *helpmenu = Gtk::manage(new Gtk::Menu());
	menuitem_help->set_submenu(*helpmenu);

	//Help
	Gtk::MenuItem *menuitem_help_ = Gtk::manage(new Gtk::MenuItem("_Help", true));
	menuitem_help_->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_help_click));
	helpmenu->append(*menuitem_help_);

	//About
	Gtk::MenuItem *menuitem_about = Gtk::manage(new Gtk::MenuItem("_About", true));
	menuitem_about->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_about_click));
	helpmenu->append(*menuitem_about);
	
	_tree_model = Gtk::ListStore::create(_columns);

	_tree_view.set_model(_tree_model);

	_tree_view.append_column("ID", _columns.m_id);
	_tree_view.append_column("Name", _columns.m_name);
	_tree_view.append_column("Value", _columns.m_value);
	_tree_view.append_column("State", _columns.m_state);
	_tree_view.append_column("Max", _columns.m_max);
	_tree_view.append_column("Min", _columns.m_min);
	
	//_tree_view.set_headers_clickable(true);
	//_tree_view.set_expander_column(_columns.m_id);
	
	_swindow.add(_tree_view);
	_swindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	_swindow.set_hexpand(true);
	_swindow.set_vexpand(true);
	vbox->pack_start(_swindow);

	//Adding rows
	Gtk::TreeModel::Row row1 = *(_tree_model->append());
	row1[_columns.m_id] = 0;
	row1[_columns.m_name] = "P0";
	row1[_columns.m_value] = 0.00;
	row1[_columns.m_max] = 0.00;
	row1[_columns.m_min] = 0.00;

	Gtk::TreeModel::Row row3 = *(_tree_model->append());
	row3[_columns.m_id] = 1;
	row3[_columns.m_name] = "P1";
	row3[_columns.m_value] = 0.00;
	row3[_columns.m_max] = 0.00;
	row3[_columns.m_min] = 0.00;
	
	Gtk::TreeModel::Row row4 = *(_tree_model->append());
	row4[_columns.m_id] = 2;
	row4[_columns.m_name] = "P2";
	row4[_columns.m_value] = 0.00;
	row4[_columns.m_max] = 0.00;
	row4[_columns.m_min] = 0.00;

	Gtk::TreeModel::Row row5 = *(_tree_model->append());
	row5[_columns.m_id] = 3;
	row5[_columns.m_name] = "Solenoid Valve";
	row5[_columns.m_state] = "CLOSED";
	row5[_columns.m_max] = 0.00;
	row5[_columns.m_min] = 0.00;


	vbox->show_all();
}

bool Mainwin::update_display()
{

	Gtk::TreeIter p0 = _tree_model->get_iter("0");
	Gtk::TreeIter p1 = _tree_model->get_iter("1");
	Gtk::TreeIter p2 = _tree_model->get_iter("2");

	Gtk::TreeIter valve = _tree_model->get_iter("3");
	
	try{
	(*p0)[_columns.m_value] = std::stof(_controller.payload->p0);
	(*p1)[_columns.m_value] = std::stof(_controller.payload->p1);
	(*p2)[_columns.m_value] = std::stof(_controller.payload->p2);

	//P0
	if(((*p0)[_columns.m_value]) > (*p0)[_columns.m_max])
		(*p0)[_columns.m_max] = std::stof(_controller.payload->p0);

	if(((*p0)[_columns.m_value]) < (*p0)[_columns.m_min])
		(*p0)[_columns.m_min] = std::stof(_controller.payload->p0);

	//P1 
	if(((*p1)[_columns.m_value]) > (*p1)[_columns.m_max])
		(*p1)[_columns.m_max] = std::stof(_controller.payload->p1);
	
	if(((*p1)[_columns.m_value]) < (*p1)[_columns.m_min])
		(*p1)[_columns.m_min] = std::stof(_controller.payload->p1);

	//P2
	if(((*p2)[_columns.m_value]) > (*p2)[_columns.m_max])
		(*p2)[_columns.m_max] = std::stof(_controller.payload->p2);

	if(((*p2)[_columns.m_value]) < (*p2)[_columns.m_min])
		(*p2)[_columns.m_min] = std::stof(_controller.payload->p2);
	}
	catch(const std::exception& e)
	{
	//	std::cerr<<"Display exception caught, verify whether transmitter is plugged in. "<<e.what()<<std::endl;
	}
	//Valve
	
	if(_controller.payload->valve_state == true)
	{
		(*valve)[_columns.m_state] = "OPEN";
		(*valve)[_columns.m_value] = 100.00;
	}
	
	else if (_controller.payload->valve_state == false)
	{
		(*valve)[_columns.m_state] = "CLOSED";
		(*valve)[_columns.m_value] = 0.00;
	}
	
	if( ( (*valve)[_columns.m_value]) > (*valve)[_columns.m_max])
		(*valve)[_columns.m_max] = 100.00;
	
	if( ( (*valve)[_columns.m_value]) < (*valve)[_columns.m_min])
		(*valve)[_columns.m_min] = 0.00;

	return true;
}
void Mainwin::on_help_click()
{
	Dialogs::image("valve.jpg");
}
void Mainwin::on_about_click()
{
    Glib::ustring s = "<span size='24000' weight='bold'>AeroMavericks Triton Telemetry System</span>\n<span size='large'>Copyright 2020 by Nihal Kumarswamy</span>\n<span size='small'>Licensed under Creative Commons Attribution 4.0 International</span>";
    Gtk::MessageDialog dlg(*this, s, true, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    dlg.run();
}
void Mainwin::on_close_valve_click()
{
	_controller.execute_cmd(4);
}
void Mainwin::on_open_valve_click()
{
	_controller.execute_cmd(3);
}
void Mainwin::on_reset_click()
{
	on_reset_p0_click();
	on_reset_p1_click();
	on_reset_p2_click();
}
void Mainwin::on_save_click()
{
	_controller.execute_cmd(5);
}
void Mainwin::on_reset_p0_click()
{
	Gtk::TreeIter p0max = _tree_model->get_iter("0");
	(*p0max)[_columns.m_max] = 0;
	
}

void Mainwin::on_reset_p1_click()
{
	Gtk::TreeIter p1max = _tree_model->get_iter("1");
	(*p1max)[_columns.m_max] = 0;
}
void Mainwin::on_reset_p2_click()
{
	Gtk::TreeIter p2max = _tree_model->get_iter("2");
	(*p2max)[_columns.m_max] = 0;
}
void Mainwin::on_openport_click()
{
	//Open Port
	_controller.execute_cmd(1);

	//Set timeouts for updating pressure & valve values
	sigc::slot<bool> my_slot = sigc::mem_fun(*this, &Mainwin::update_display);
	Glib::signal_timeout().connect(my_slot, 100);
}

void Mainwin::on_open_1s_click()
{
	//Opens valve 
	_controller.execute_cmd(3);
	
	//create a std::thread to close after 1s 
	std::thread close_thread([this] () {close_after(1);});
	close_thread.detach();
}

void Mainwin::on_open_5s_click()
{
	//Opens valve 
	_controller.execute_cmd(3);
	
	//create a std::thread to close after 5s 
	std::thread close_thread([this] () {close_after(5);});
	close_thread.detach();
}

void Mainwin::on_open_10s_click()
{
	//Opens valve 
	_controller.execute_cmd(3);
	
	//create a std::thread to close after 10s 
	std::thread close_thread([this] () {close_after(10);});
	close_thread.detach();
}
void Mainwin::on_open_15s_click()
{
	//Opens valve 
	_controller.execute_cmd(3);
	
	//create a std::thread to close after 15s 
	std::thread close_thread([this] () {close_after(15);});
	close_thread.detach();
}
void Mainwin::close_after(int sec)
{
	usleep(1000000*sec);

	_controller.execute_cmd(4);
}
void Mainwin::on_closeport_click()
{
	_controller.execute_cmd(2);
}

void Mainwin::on_close_click()
{
	hide();
}

Mainwin::~Mainwin() {}
