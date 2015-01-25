#include "Sync_tcp_client.h"


Sync_tcp_client::Sync_tcp_client(boost::asio::io_service& ioserver, std::string& host, std::string& port)
:m_socket(ioserver)
, m_resolver(ioserver)
{
	Start(host, port);
}


Sync_tcp_client::~Sync_tcp_client()
{
	if (m_socket.is_open())
	{
		m_socket.close();
	}
}

void Sync_tcp_client::Start(std::string& host, std::string& port)
{
	boost::asio::connect(m_socket, m_resolver.resolve({ host, port }));
	while (true)
	{
		std::cout << "Enter message: ";
		char request[MAX_BUFFER];
		std::cin.getline(request, MAX_BUFFER);

		size_t request_length = std::strlen(request);
		boost::asio::write(m_socket, boost::asio::buffer(request, request_length));

		char reply[MAX_BUFFER];
		size_t reply_length = boost::asio::read(m_socket,
			boost::asio::buffer(reply,request_length));

		std::cout << "Reply is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
}


