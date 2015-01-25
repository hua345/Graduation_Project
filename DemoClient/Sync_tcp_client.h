#pragma once
#include <boost/asio.hpp>
#include <iostream>


class Sync_tcp_client
{
public:
	static const int  MAX_BUFFER = 1024;
	Sync_tcp_client(boost::asio::io_service& ioserver, std::string& host, std::string& port);
	void Start(std::string& host, std::string& port);
	~Sync_tcp_client();
private:


	boost::asio::ip::tcp::socket m_socket;
	boost::asio::ip::tcp::resolver m_resolver;


};

