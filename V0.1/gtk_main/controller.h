#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "chat_client.h"

class Controller{
	
	public:
		Controller();
		void execute_cmd(int cmd);
		Payload *payload;
	private:
		chat_client *c;
		std::string ip, port, file_name;
		void open_port(std::string _ip, std::string _port, std::string _file_name);
		void close_port();
		
		

};
#endif
