#include "chat_client.h"
#include "global.h"

using asio::ip::tcp;

chat_client::chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints, std::string file_name)
    : io_context_(io_context),
      socket_(io_context)
  {
    
    tmp = new logger(file_name);
    do_connect(endpoints);
  }
chat_client::~chat_client()
{
  free(tmp);
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
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            //printf("%c",read_msg_.body()[2]);
            std::cout << "\n";
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
