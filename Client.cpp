#include "Client.hpp"

Client::Client() : ioc(), // Create our I/O context,
	resolver(ioc), // Initialize the resolver
	socket(ioc) // Initialize the socket
{
}
