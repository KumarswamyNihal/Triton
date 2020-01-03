#include <string.h>

#include "chat_client.h"
#include "global.h"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

  chat_client::chat_client(std::string ip, std::string port, std::string file_name)
    : socket_(io_context_)
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
  free(data);
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
            /* std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n"; */
            if(containsP(read_msg_.body_length(), (unsigned char*)read_msg_.body()) == 1 && validatepacket(read_msg_.body_length(), (unsigned char*)read_msg_.body()) == 1){
            int tmp = read_msg_.body()[4]<<8+read_msg_.body()[3], tmp_conv;
            g_mutex_lock(P0_mutex);
            sprintf(P0,"%3.2f",tmp);
            g_mutex_unlock(P0_mutex);

            g_mutex_lock(P0_max_mutex);
            sscanf(P0_max, "%d", &tmp_conv);
            if(tmp>tmp_conv)
              sprintf(P0_max, "%3.2f", tmp);
            g_mutex_unlock(P0_max_mutex);

            tmp = read_msg_.body()[5]+read_msg_.body()[6]<<8;
            g_mutex_lock(P1_mutex);
            sprintf(P1,"%3.2f",tmp);
            g_mutex_unlock(P1_mutex);

            g_mutex_lock(P1_max_mutex);
            sscanf(P1_max, "%d", &tmp_conv);
            if(tmp>tmp_conv)
              sprintf(P1_max, "%3.2f", tmp);
            g_mutex_unlock(P1_max_mutex);
            }
            char tmp[90];
            g_mutex_lock(P0_mutex);
            strcpy(tmp, P0);
            g_mutex_unlock(P0_mutex);
            strcat(tmp, ", ");
            g_mutex_lock(P1_mutex);
            strcat(tmp, P1);
            g_mutex_unlock(P1_mutex);
            strcat(tmp, ", ");
            g_mutex_lock(valve_mutex);
            if(valve_state == true)
              strcat(tmp, "1");
            else if(valve_state == false)
              strcat(tmp, "0");
            g_mutex_unlock(valve_mutex); 
            data->log(tmp);
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


/* int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

     asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); }); 
    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
} */
