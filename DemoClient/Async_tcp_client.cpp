#include "Async_tcp_client.h"


Async_tcp_client::Async_tcp_client(boost::asio::io_service& ioserver, std::string& host, std::string& port)
:m_socket(ioserver)
, m_resolver(ioserver)
, m_isConnected(false)
{
	m_itendpoint = m_resolver.resolve({ host, port });
	do_connect(host, port);
}


Async_tcp_client::~Async_tcp_client()
{
	close_connect();
}

void Async_tcp_client::do_connect(std::string& host, std::string& port)
{
	//callback by lambda 
	boost::asio::async_connect(m_socket, m_itendpoint
		,boost::bind(&Async_tcp_client::handle_connect,this, boost::asio::placeholders::error));

}
void Async_tcp_client::close_connect()
{
	if (m_socket.is_open())
	{
		m_socket.close();
	}
	// tell the IO service to stop
	m_socket.get_io_service().stop();

	m_isConnected = false;
}
void Async_tcp_client::do_reconnect(const boost::system::error_code& error)
{
	if (m_isConnected) return;
	// close current socket if necessary
	if (m_socket.is_open())
		m_socket.close();

	//callback by lambda 
	boost::asio::async_connect(m_socket, m_itendpoint
		, boost::bind(&Async_tcp_client::handle_connect,this, boost::asio::placeholders::error));
}
//connect callback
void Async_tcp_client::handle_connect(const boost::system::error_code& error)
{
	if (!error)
	{
		//we are connected
		m_isConnected = true;

		//read && write
		m_socket.async_write_some(boost::asio::buffer("link successed!")
			, boost::bind(&Async_tcp_client::handle_write, this
			, boost::asio::placeholders::error
			, boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		// there was an error :(
		m_isConnected = false;


	}
}
void Async_tcp_client::handle_read(const boost::system::error_code& e, size_t byte_transferred)
{
	if (!e)
	{
		//recv next data
		m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER),
			boost::bind(&Async_tcp_client::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		std::cout << "write error : " << e.message() << std::endl;
		m_socket.close();
	}
}

void Async_tcp_client::handle_write(const boost::system::error_code& e, size_t bytes_transferred)
{
	if (!e)
	{

		std::cout << "receive from server : " << m_recvBuf << std::endl;

		memset(m_recvBuf, 0, 1024);
		//get string  from cmd
		std::cout << "Enter message: ";
		memset(m_sendBuf, 0, MAX_BUFFER);
		std::cin.getline(m_sendBuf, MAX_BUFFER);

		size_t m_buflength = strlen(m_sendBuf) + 1;
		//return a message to client
		m_socket.async_write_some(boost::asio::buffer(m_sendBuf, strlen(m_sendBuf) + 1),
			boost::bind(&Async_tcp_client::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		std::cout << "read error : " << e.message() << std::endl;
		m_socket.close();
	}
}