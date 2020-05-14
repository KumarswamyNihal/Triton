/*TODO
 *Move chat client stuff to chat client.cpp
 *Build GUI with PO/1 MAX LAST TIME STAMP AND OPEN CLOSE BUTTONS
 *
 */
#include <stdio.h>
#include <gtk/gtk.h>
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"
 #include "global.h"

#define __STDC_FORMAT_MACROS
//#define ASIO_STANDALONE
#define PACKET_START_BYTE  0xAA
using asio::ip::tcp;


typedef std::deque<chat_message> chat_message_queue;
void ObtainGuiWidgets(GtkBuilder *p_builder)
{
  #define GuiappGET(xx) gui_app->xx=GTK_WIDGET(gtk_builder_get_object(p_builder,#xx))
  GuiappGET(window1);
    
}
class chat_client
{
public:
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            //std::cout.write(read_msg_.body(), read_msg_.body_length());
			printf("%s", read_msg_.body());
            std::cout << "\n";
            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }

private:
  asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

int validatepacket(int packetSize, unsigned char *buffer)
{
	if(buffer[0] != PACKET_START_BYTE)
	{
		printf("VALIDATE BIG%c\n", buffer[0]);
		return 0;

	}
		
	if(buffer[1] != packetSize)
	{
		
		return 0;
	}

	if(buffer == NULL)
	{
		
		return 0;
	}
	unsigned char checksum = 0x00;
	for(unsigned int i=0; i < packetSize-1; i++)
		checksum = checksum ^ buffer[i];
	
	if(buffer[packetSize-1] != checksum){
		printf("%c", checksum);
		return 0; 
	}

	return 1;
}
int main(int argc, char* argv[])
{
	GtkBuilder *builder;
  GError *err = NULL;
  gtk_init(&argc, &argv);
  
  //create gtk_instance for visualization
  gui_app = g_slice_new(Gui_Window_AppWidgets);

  //builder
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "sprint1.glade", &err);

  if(err)
    {
      g_error(err->message);
      g_error_free(err);
      g_slice_free(Gui_Window_AppWidgets, gui_app);
      exit(-1);
    }

  // Obtain widgets that we need
  ObtainGuiWidgets(builder);

// Connect signals
  gtk_builder_connect_signals(builder, gui_app);

  // Destroy builder now that we created the infrastructure
  g_object_unref(G_OBJECT(builder));

  //display the gui
  gtk_widget_show(GTK_WIDGET(gui_app->window1));

  

	asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], "5000");
    chat_client c(io_context, endpoints); 
	
    std::thread t([&io_context](){ io_context.run(); });
	
	//const unsigned char PACKET_START_BYTE = 0xAA;
	int ser_dev, count = 0, packetSize = 0;
	size_t res;
	struct termios my_serial;
	unsigned char ob[50];
	//char ob[50];
	unsigned char buffer[255];
	ser_dev = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
	bzero(&my_serial, sizeof(my_serial));
	my_serial.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	tcflush(ser_dev, TCIFLUSH);
	tcsetattr(ser_dev, TCSANOW, &my_serial);

		
		while(1){
			//char buff[10];
							/* buff[0] = 0xAA;
							buff[1] = 8;
							buff[2] = 0x50;
							buff[3] = 0x70;
							buff[4] = 0x00;
							buff[5] = 0x6F;
							buff[6] = 0x00;
							buff[7] = 0xED;
							write(ser_dev, buff, 8); */
			if(ser_dev != -1){
				res = read(ser_dev, ob, 1);
				
				if(res == 0)
				usleep(1000);
				
				else{//TODO: VALIDATE
					if(count == 0 && ob[0] == PACKET_START_BYTE)
					{
						
						buffer[count] = ob[0];
						count++;
						continue;
					}
					else if(count == 0)
					continue;

					else if(count == 1){
						buffer[count] = ob[0];
						packetSize = ob[0];
						count++;
						continue;
					}
					else if(count<packetSize){
						buffer[count] = ob[0];
						count++;
					}
					if(count >= packetSize){
						if(validatepacket(packetSize, buffer) == 1){
							//Received a packet
							//printf("%d\n", buffer[1]);
							 chat_message msg;
      						msg.body_length(packetSize);
      						strncpy(msg.body(), (const char*)buffer, packetSize);
							//printf("buffer is: %c\n", buffer[4]);
      						msg.encode_header();
      						c.write(msg); 
							
						}
						else
						{
							printf("VALIDATE FAILED\n");
						}
						
						count = 0;
						packetSize = 0;
					}
				}
			}
			else
			{
				usleep(100000);	//Sleep between threads. Need to decrease time
			}
		}
	c.close();
    t.join();
	gtk_main();
	return 0;
}

#include <iostream>
#include <ctime>
#include <string.h>
int main ()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[20];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
  std::string str(buffer);

  std::cout <<str;