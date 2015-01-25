#include "Ping.h"


Ping::Ping(boost::asio::io_service& io_service, const char* destination)
: resolver_(io_service), socket_(io_service, icmp::v4()),
m_timer(io_service), sequence_number_(0), num_replies_(0)
, m_currentProcId(0)
{
	m_currentProcId = get_identifier();
	icmp::resolver::query query(icmp::v4(), destination, "");
	destination_ = *resolver_.resolve(query);

	start_send();
	start_receive();
}

void Ping::start_send()
{
	std::string body("PING");

	// Create an ICMP header for an echo request.
	icmp_header echo_request;
	echo_request.type(icmp_header::echo_request);
	echo_request.code(0);
	echo_request.identifier(m_currentProcId);
	echo_request.sequence_number(++sequence_number_);
	compute_checksum(echo_request, body.begin(), body.end());

	// Encode the request packet.
	boost::asio::streambuf request_buffer;
	std::ostream os(&request_buffer);
	os << echo_request << body;

	// Send the request.
	time_sent_ = boost::posix_time::microsec_clock::universal_time();
	socket_.send_to(request_buffer.data(), destination_);

	// Wait up to five seconds for a reply.
	num_replies_ = 0;
	m_timer.expires_at(time_sent_ + boost::posix_time::seconds(5));
	m_timer.async_wait(boost::bind(&Ping::handle_timeout, this));
}
void Ping::handle_timeout()
{
	if (num_replies_ == 0)
		std::cout << "Request timed out" << std::endl;

	// Requests must be sent no less than one second apart.
	m_timer.expires_at(time_sent_ + boost::posix_time::seconds(1));
	m_timer.async_wait(boost::bind(&Ping::start_send, this));
}
void Ping::start_receive()
{
	// Discard any data already in the buffer.
	reply_buffer_.consume(reply_buffer_.size());

	// Wait for a reply. We prepare the buffer to receive up to 64KB.
	socket_.async_receive(reply_buffer_.prepare(65536),
		boost::bind(&Ping::handle_receive, this, boost::asio::placeholders::bytes_transferred));
}
void Ping::handle_receive(std::size_t length)
{
	// The actual number of bytes received is committed to the buffer so that we
	// can extract it using a std::istream object.
	reply_buffer_.commit(length);

	// Decode the reply packet.
	std::istream is(&reply_buffer_);
	ipv4_header m_ipv4_hdr;
	icmp_header m_icmp_hdr;
	is >> m_ipv4_hdr >> m_icmp_hdr;

	// We can receive all ICMP packets received by the host, so we need to
	// filter out only the echo replies that match the our identifier and
	// expected sequence number.
	if (is && m_icmp_hdr.type() == icmp_header::echo_reply
		&& m_icmp_hdr.identifier() == m_currentProcId
		&& m_icmp_hdr.sequence_number() == sequence_number_)
	{
		// If this is the first reply, interrupt the five second timeout.
		if (num_replies_++ == 0)
			m_timer.cancel();

		// Print out some information about the reply packet.
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
		std::cout << length - m_ipv4_hdr.header_length()
			<< " bytes from " << m_ipv4_hdr.source_address()
			<< ": icmp_seq=" << m_icmp_hdr.sequence_number()
			<< ", ttl=" << m_ipv4_hdr.time_to_live()
			<< ", time=" << (now - time_sent_).total_milliseconds() << " ms"
			<< std::endl;
	}

	start_receive();
}
 unsigned short Ping::get_identifier()
{
#if defined(BOOST_ASIO_WINDOWS)
	return static_cast<unsigned short>(::GetCurrentProcessId());
#else
	return static_cast<unsigned short>(::getpid());
#endif
}