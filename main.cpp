/* Standard headers */
#include <iostream> // std::clog

/* Boost */
#include <boost/asio/io_context.hpp> // boost::asio::io_context
#include <boost/system/error_code.hpp> // boost::system::error_code
#include <boost/asio/ip/tcp.hpp> // boost::asio::ip::tcp::resolver, boost::asio::ip::tcp::socket, boost::asio::ip::tcp::resolver::results_type, boost::asio::ip::tcp::endpoint
#include <boost/asio/connect.hpp> // boost::asio::connect

int main()
{
	boost::asio::io_context ioc; // For all I/O ops
	boost::asio::ip::tcp::resolver resolver(ioc); // To convert a hostname to a list of endpoints
	typename boost::asio::ip::tcp::resolver::results_type resolveRes = resolver.resolve("cat-fact.herokuapp.com", "80"); // Get a list of HTTP endpoints
	boost::asio::ip::tcp::socket sock(ioc); // THe socket over which we communicate with the server
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

	boost::asio::ip::tcp::endpoint connectedEndpoint = boost::asio::connect(sock, resolveRes); // Connect to an endpoint

	/* Print endpoint details to prove that it connected to one */
	std::clog << "Connected to an endpoint." << std::endl
	<< "\tAddress: " << connectedEndpoint.address().to_string() << std::endl
	<< "\tCapacity: " << connectedEndpoint.capacity() << std::endl
	<< "\tPort: " << connectedEndpoint.port() << std::endl
	<< "\tSize: " << connectedEndpoint.size() << std::endl;
	return 0;
}
