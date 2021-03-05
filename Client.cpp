/* Standard headers */
#include <iostream> // std::cerr
#include <iomanip> // std::quoted
#include <memory> // std::make_unique
#include <thread> // std::thread

/* Boost */
#include <boost/system/error_code.hpp> // boost::system::error_code
#include <boost/asio/ip/tcp.hpp> // boost::asio::ip::tcp::resolver::results_type, boost::asio::ip::tcp::endpoint

/* Our headers */
#include "Client.hpp" // Class def.

Client::Client() : ioc(), // Create our I/O context,
	resolver(ioc), // Initialize the resolver
	socket(ioc) // Initialize the socket
{
	resolveRes = resolver.resolve("cat-fact.herokuapp.com", "80"); // Convert our domain name and port to a list of endpoints
	int eNo = 1;
	for (boost::asio::ip::tcp::endpoint endPoint : resolveRes)
	{
		std::clog << "Endpoint #" << eNo << std::endl
		<< "\tAddress: " << endPoint.address().to_string() << std::endl
		<< "\tCapacity: " << endPoint.capacity() << std::endl
		<< "\tPort: " << endPoint.port() << std::endl
		<< "\tSize: " << endPoint.size() << std::endl
		<< std::endl;
		++eNo;
	}
}
