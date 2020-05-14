#include <gtkmm.h>
#include <stdexcept>

#include "controller.h"
#include "dialogs.h"

/*Constructor*/
Controller::Controller() {}

void Controller::open_port(std::string _ip, std::string _port, std::string _file_name)
{	
	try{
		c = new chat_client(_ip, _port, _file_name, payload);

		}
	catch(std::exception& e){
		Dialogs::message("Error: Cannot create chat_client");
		std::cerr<<"Cannot create chat_client"<<std::endl;

		}
}

void Controller::close_port()
{
	c->close();

	delete c;
}

void Controller::execute_cmd(int cmd)
{
	switch(cmd) {
		case 1: {//Create chat_client
					ip = Dialogs::input("Enter IP Address"
							, "Input", "IP Address");
					port = Dialogs::input("Enter Port number"
							, "Input", "5000");
					file_name = Dialogs::input("Enter filename to save", "Input", "data.dat");
				
					payload = new Payload;

					open_port(ip, port, file_name);
					
					break;
				}
		case 2: {//Close chat_client
					close_port();
					
					break;
				}
		case 3: {//Open valve
					chat_message msg;
					msg.body_length(2);
					strncpy(msg.body(), "VT", 2);
					msg.encode_header();
					
					payload->valve_state = true;
					c->write(msg);
					
					break;
				}
		case 4: {//Close valve

				chat_message msg;
				msg.body_length(2);
				strncpy(msg.body(), "VF", 2);
				msg.encode_header();
				payload->valve_state = false;
				
				c->write(msg);

				break;
				}
		case 5: {//Change filename 
				if(c != NULL){
					close_port();
					file_name = Dialogs::input("Enter filename to save", "Input", "data.dat");
					open_port(ip, port, file_name);
				}
				else{
					Dialogs::message("Error: Cannot create chat_client");
				}

				break;
		}
	}
}
