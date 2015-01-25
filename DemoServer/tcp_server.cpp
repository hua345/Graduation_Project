
#include "stdafx.h"

#include "tcp_server.h"
//tcp constructor
tcp_server::tcp_server(boost::asio::io_service& io_service, const boost::asio::ip::tcp::endpoint& endpoint)
: m_acceptor(io_service, endpoint)
{
	start_accept();
}
void tcp_server::start_accept()
{
	//生成一个指向对话实例的指针
	auto m_session = std::make_shared<Tcp_Session>(m_acceptor.get_io_service());

	m_acceptor.async_accept(m_session->socket(),
		boost::bind(&tcp_server::handle_accept, this, m_session,
		boost::asio::placeholders::error));
}
void tcp_server::handle_accept(std::shared_ptr<Tcp_Session> Session,
	const boost::system::error_code& error)
{
	if (!error)
	{

		Session->start();
		//accept new client connection
		start_accept();
	}
}