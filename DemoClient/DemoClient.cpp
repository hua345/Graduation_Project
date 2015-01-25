#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "Sync_tcp_client.h"
#include "Async_tcp_client.h"
#include "Ping.h"
using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;
		std::string m_host("www.baidu.com");
		std::string m_port("80");

		//Async_tcp_client m_client(io_service, m_host, m_port);
		Ping p(io_service, m_host.c_str());
		//async begin
		io_service.run();

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	system("PAUSE");
	return 0;
}