// DemoServer.cpp : �������̨Ӧ�ó������ڵ㡣
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
		//�����õ���inet4

		boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::tcp::v4(), port);
		//�նˣ����Կ���sockaddr_in����ɺ󣬾�Ҫaccept��

		tcp_server sa(m_service, endPoint);
		//�����շ��߼�

		m_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	system("PAUSE");
	return 0;
}

