
#include "stdafx.h"

#include "Tcp_Session.h"
//private constructor
Tcp_Session::Tcp_Session(boost::asio::io_service& io_service)
: m_socket(io_service)
{
	memset(m_recvBuf, 0, 1024);
}
//free resource
Tcp_Session::~Tcp_Session()
{
	if (m_socket.is_open())
	{
		m_socket.close();
	}
}
//connection start function
void Tcp_Session::start()
{
	//first connection return flags
	m_socket.async_write_some(boost::asio::buffer("link successed!")
		, boost::bind(&Tcp_Session::handle_write, shared_from_this()
		, boost::asio::placeholders::error
		, boost::asio::placeholders::bytes_transferred));

	//get client IP address
	std::cout << "get new client  " << m_socket.remote_endpoint().address() << std::endl;

	m_sendBuf = make_daytime_string();
}
//read callback function
void Tcp_Session::handle_read(const boost::system::error_code& e
	, size_t byte_transferred)
{
	if (!e)
	{
		std::cout << "receive from client : " << m_recvBuf << std::endl;

		memset(m_recvBuf, 0, 1024);
		//return a message to client
		m_socket.async_write_some(boost::asio::buffer(m_sendBuf.c_str(), m_sendBuf.size() + 1),
			boost::bind(&Tcp_Session::handle_write, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		std::cout << "read error : " << e.message() << std::endl;
		m_socket.close();
	}

}
//write callback function
void Tcp_Session::handle_write(const boost::system::error_code& e,
	size_t bytes_transferred)
{
	if (!e)
	{
		//recv next data
		m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER),
			boost::bind(&Tcp_Session::handle_read, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		std::cout << "write error : " << e.message() << std::endl;
		m_socket.close();
	}

}