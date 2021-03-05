#ifndef CLIENT_HPP
#define CLIENT_HPP

/* Boost */
#include <boost/asio/io_context.hpp> // boost::asio::io_context
#include <boost/asio/ip/tcp.hpp> // boost::asio::ip::tcp::resolver, boost::asio::ip::tcp::socket

class Client
{
	public:
		Client();

	private:
		boost::asio::io_context ioc; // Needed by Boost.Asio for all operations.
		boost::asio::ip::tcp::resolver resolver;
		boost::asio::ip::tcp::socket socket;
};

#endif
