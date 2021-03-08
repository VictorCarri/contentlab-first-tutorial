/* Standard C++ */
#include <utility> // std::pair
#include <vector> // std::vector
#include <string> // std::string

/* Boost */
#include <boost/asio.hpp> // boost::asio::const_buffer

/* Our headers */
#include "Request.hpp" // Class def'n

/**
* @desc Constructor. Initializes const fields.
**/
Request::Request() : crlf {'\r', '\n'}, // Initialize the line-ender buffer
    nameValSep {':', ' '} // Initialize the separator for header names and values
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
* @desc Converts the Request object to a vector of buffers that can be sent over the network.
* @return A vector of buffers that can be passed to Boost.Asio's write function.
**/
std::vector<boost::asio::const_buffer> Request::toBuffers()
{
    /* Clear the old data, just in case */
    bufs.clear();
    sdata.clear();

    /* Write the request line (method, resource, and HTTP version identifier) */
    sdata.push_back("GET /facts/random  HTTP/1.1"); // Tells the API to return a random fact
    bufs.push_back(boost::asio::buffer(sdata.back())); // Push back a buffer with the request line
    bufs.push_back(boost::asio::buffer(crlf)); // End the request line

    /* Write the request's headers */
    for (std::pair<std::string, std::string> header : headers) // Loop through the headers in order
    {
        /* Write the header name and value, separated by a single colon and a space */
        sdata.push_back(header.first); // Store the name
        bufs.push_back(boost::asio::buffer(sdata.back())); // Add a buffer for the header's name
        bufs.push_back(boost::asio::buffer(nameValSep)); // Add a buffer for the separator
        sdata.push_back(header.second); // Store the header's value
        bufs.push_back(boost::asio::buffer(sdata.back())); // Add a buffer for the header's value
        bufs.push_back(boost::asio::buffer(crlf)); // Add the line terminator
    }

    bufs.push_back(boost::asio::buffer(crlf)); // Add a buffer for the request terminator
    return bufs; // Return a copy of our buffers. We store them ourselves to ensure that they don't disappear before the caller sends them over the network using Boost.Asio
}
