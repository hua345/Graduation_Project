#ifndef DEMO_TCP_SESSION
#define DEMO_TCP_SESSION

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#endif
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory.h>
#include <iostream>

using namespace boost::asio::ip;
static const  unsigned int MAX_BUFFER = 1024;

//We will use shared_ptr and enable_shared_from_this because we want to 
//keep the tcp_connection object alive as long as there is an operation that refers to it.
class Tcp_Session
	: public std::enable_shared_from_this<Tcp_Session>
{
public:
	//return current socket refference
	tcp::socket& socket()
	{
		return m_socket;
	}
	//async_write to serve the data to the client.
	void start();
	//return daytime string
	std::string make_daytime_string()
	{
		using namespace std; // For time_t, time and ctime;
		time_t now = time(0);
		return ctime(&now);
	}
	//private constructor
	Tcp_Session(boost::asio::io_service& io_service);
	~Tcp_Session();
private:

	//read callback function
	void handle_read(const boost::system::error_code& e
		, size_t byte_transferred);
	//write callback function
	void handle_write(const boost::system::error_code& e,
		size_t bytes_transferred);
	//The current connection socket
	tcp::socket m_socket;
	//send message data
	std::string m_sendBuf;
	//Buffer
	char m_recvBuf[1024];
};