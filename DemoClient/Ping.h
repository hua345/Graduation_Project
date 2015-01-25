#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "icmp_header.hpp"
#include "ipv4_header.hpp"

using boost::asio::ip::icmp;
using boost::asio::deadline_timer;
class Ping
{
public:
	Ping(boost::asio::io_service& io_service, const char* destination);
	~Ping(){}
private:
	void start_send();

	void handle_timeout();

	void start_receive();

	void handle_receive(std::size_t length);
	
	static unsigned short get_identifier();

	icmp::resolver resolver_;				
	icmp::endpoint destination_;			//remote address
	icmp::socket socket_;					//current socket
	deadline_timer m_timer;					//deadline time
	unsigned short sequence_number_;		//sequence_number_
	boost::posix_time::ptime time_sent_;	//send time
	boost::asio::streambuf reply_buffer_;	
	std::size_t num_replies_;
	unsigned short m_currentProcId;			//current process ID

};

