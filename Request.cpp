/* Standard C++ */
#include <utility> // std::pair
#include <vector> // std::vector
#include <string> // std::string
#include <ostream> // std::ostream

/* Boost */
#include <boost/asio.hpp> // boost::asio::const_buffer

/* Our headers */
#include "Request.hpp" // Class def'n

/**
* @desc Constructor. Initializes const fields.
**/
Request::Request() : crlf("\r\n"), // Initialize the line-ender string
    nameValSep(": ") // Initialize the separator for header names and values
{
}

/*
* @desc Adds a new header to the request.
* @param name The header's name.
* @param val The header's value.
**/
void Request::addHeader(std::string name, std::string val)
{
    headers.emplace_back(name, val); // Construct a new pair in-place at the start of the list
}

/**
* @desc Returns the request's internal buffer for Boost.Asio's write() function.
**/
boost::asio::streambuf& Request::getBuf()
{
	return reqBuf;
}

/**
* @desc Tells the Request object to convert itself to a streambuf for writing.
**/
void Request::createBuf()
{
	reqBuf.consume(reqBuf.capacity()); // Clear the buffer in case it contained anything
	std::ostream reqStream(&reqBuf); // The stream we will use to write the request
	reqStream << "GET /facts/random HTTP/1.0" << crlf; // Write the request line

	for (std::pair<std::string, std::string> header : headers) // Write each header
	{
		reqStream << header.first << nameValSep << header.second << crlf; // Write the header's name, a separator, the value, and a CRLF terminator
	}

	reqStream << crlf; // End the request
}
