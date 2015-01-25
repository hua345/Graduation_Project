// DemoServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <boost/asio.hpp>
#include "tcp_server.h"
#include "UDP_Server.h"

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		boost::asio::io_service m_service;
		short port = 8080/*argv[1]*/;
		//我们用的是inet4

		boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::tcp::v4(), port);
		//终端（可以看作sockaddr_in）完成后，就要accept了

		tcp_server sa(m_service, endPoint);
		//数据收发逻辑

		m_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	system("PAUSE");
	return 0;
}

