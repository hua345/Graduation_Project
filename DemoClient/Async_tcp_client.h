#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <boost/bind.hpp>

class Async_tcp_client
{
public:
	static const int  MAX_BUFFER = 1024;
	Async_tcp_client() = delete;
	Async_tcp_client(boost::asio::io_service& ioserver, std::string& host, std::string& port);
	~Async_tcp_client();
	//connect operate
	void do_reconnect(const boost::system::error_code& error);

	void close_connect();
private:
	//connect operate
	void do_connect(std::string& host, std::string& port);
	
	void handle_connect(const boost::system::error_code& error);
	//read && write
	void do_read(size_t bytes_transferred);
	void do_write();

	//read callback function
	void handle_read(const boost::system::error_code& error
		, size_t byte_transferred);
	//write callback function
	void handle_write(const boost::system::error_code& error,
		size_t bytes_transferred);

	boost::asio::ip::tcp::socket m_socket;
	boost::asio::ip::tcp::resolver m_resolver;
	//query address list
	boost::asio::ip::tcp::resolver::iterator m_itendpoint;
	//connected state
	bool m_isConnected;
	//buffer
	char m_recvBuf[MAX_BUFFER];
	char m_sendBuf[MAX_BUFFER];

};

