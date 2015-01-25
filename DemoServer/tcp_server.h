#ifndef DEMO_TCP_SERVER
#define DEMO_TCP_SERVER

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#endif
#include <boost/asio.hpp>
#include <memory>
#include "Tcp_Session.h"
class tcp_server
{
public:
	//constructor
	tcp_server(boost::asio::io_service& io_service,const boost::asio::ip::tcp::endpoint& endpoint);

private:
	//async accpet connection
	void start_accept();
	//accept callback function
	void handle_accept(std::shared_ptr<Tcp_Session> new_connection,
		const boost::system::error_code& error);
	//used for accepting new socket connections
	tcp::acceptor m_acceptor;
};