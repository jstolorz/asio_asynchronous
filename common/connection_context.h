#pragma once

#include "global.h"

class Connection_Context : public boost::enable_shared_from_this<Connection_Context>{

public:
    Connection_Context(boost::asio::io_service& service)
    : m_socket(service), m_recv_buffer_index(0)
    {}

    ~Connection_Context(){}

private:
    boost::asio::ip::tcp::socket m_socket;
    std::vector<boost::uint8_t> m_recv_buffer;
    size_t m_recv_buffer_index;
    std::list<std::vector<boost::uint8_t>> m_send_buffer;
};