#ifndef CLIENT_HPP
#define CLIENT_HPP

/* Boost */
#include <boost/asio/io_context.hpp> // boost::asio::io_context
#include <boost/asio/ip/tcp.hpp> // boost::asio::ip::tcp::resolver, boost::asio::ip::tcp::socket, boost::asio::ip::tcp::resolver::results_type

class Client
{
	public:
		/*
		* Constructor. Sets up Asio variables and connects to the server.
		*/
		Client();

	private:
		boost::asio::io_context ioc; // Needed by Boost.Asio for all operations.
		boost::asio::ip::tcp::resolver resolver; // Converts our domain name and port to an endpoint object that Boost.Asio can understand
		boost::asio::ip::tcp::socket socket; // The socket over which we will communicate with the server
		typename boost::asio::ip::tcp::resolver::results_type resolveRes; // List of endpoints associated with our domain name and port
};

#endif
