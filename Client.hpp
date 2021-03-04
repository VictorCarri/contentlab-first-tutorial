#ifndef CLIENT_HPP
#define CLIENT_HPP

/* Boost */
#include <boost/asio/io_context.hpp> // boost::asio::io_context

class Client
{
	public:
		Client();

	private:
		boost::asio::io_context ioc; // Needed by Boost.Asio for all operations.
};

#endif
