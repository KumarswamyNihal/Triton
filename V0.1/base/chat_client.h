#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"

#include "logger.h"
#include "packets.h"

#pragma once

typedef std::deque<chat_message> chat_message_queue;
using asio::ip::tcp;

class chat_client
{
    public:
        chat_client(std::string ip, std::string port, std::string file_name);
        ~chat_client();
        void write(const chat_message& msg);
        void close();

    private:
        void do_connect(const tcp::resolver::results_type& endpoints);
        void do_read_header();
        void do_read_body();
        void do_write();
        asio::io_context io_context_;
        tcp::socket socket_;
        chat_message read_msg_;
        chat_message_queue write_msgs_;
        logger *data;
        asio::ip::basic_resolver_results<asio::ip::tcp> endpoints;
        std::thread *t;
        
};