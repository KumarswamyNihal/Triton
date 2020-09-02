#include <string.h>
#include <string>

#include "chat_client.h"
#include "packets.h"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

chat_client::chat_client(std::string ip, std::string port
		, std::string file_name, Payload *payload)
    : socket_(io_context_), payload_(payload)
  {
    data = new logger(file_name);
	tcp::resolver resolver(io_context_);
    endpoints = resolver.resolve(ip, port);
   
   	do_connect(endpoints);
    t = new std::thread([this](){ io_context_.run(); });
  }

  chat_client::~chat_client()
{
  t->join();
}
  void chat_client::write(const chat_message& msg)
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

  void chat_client::close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }


  void chat_client::do_connect(const tcp::resolver::results_type& endpoints)
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

  void chat_client::do_read_header()
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

  void chat_client::do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            //std::cout.write(read_msg_.body(), read_msg_.body_length());
            //std::cout << "\n";
          
          if (containsP(read_msg_.body_length(), (unsigned char*) read_msg_.body()) == 1){
			
			      unsigned int tmp = ((unsigned char)read_msg_.body()[3]) + ((unsigned char)read_msg_.body()[4]<<8);
            //check constants
            float tmp_f = (((tmp/4096.0)*3.3)*55.5566) - 16.666666;
            
            char tmp_c[80];
            sprintf(tmp_c, "%3.3f", tmp_f);
            payload_->p0 = std::string(tmp_c);

            tmp = ((unsigned char)read_msg_.body()[6]<<8) + ((unsigned char)read_msg_.body()[5]);
            
            tmp_f = (((tmp/4096.0)*3.3)*55.5566) - 16.666666;
            
            sprintf(tmp_c, "%3.3f", tmp_f);
            payload_->p1 = std::string(tmp_c);

			tmp = ((unsigned char)read_msg_.body()[8]<<8) + ((unsigned char)read_msg_.body()[7]);
			tmp_f = (((tmp/4096.0)*3.3)*55.5566)-16.66666;

			sprintf(tmp_c, "%3.3f", tmp_f);
			payload_->p2 = std::string(tmp_c);

          }
          if(payload_->valve_state == false)
            data->log(payload_->p0+", "+payload_->p1+", "+payload_->p2+", "+"0");
          else if(payload_->valve_state == true)
            data->log(payload_->p0+", "+payload_->p1+", "+payload_->p2+", "+"1");
            do_read_header();
          }
          else
          {
			
            socket_.close();
          }
        });
  }

  void chat_client::do_write()
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
