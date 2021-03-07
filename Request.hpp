#ifndef REQUEST_HPP
#define REQUEST_HPP

/* Standard C++ */
#include <ostream> // std::ostream
#include <forward_list> // std::forward_list
#include <utility> // std::pair
#include <string> // std::string
#include <vector> // std::vector

/* Boost */
#include <boost/asio.hpp> // boost::asio::const_buffer

class Request
{
	public:
		/**
		* @desc Adds a new header to the request.
		* @param name The header's name.
		* @param val The header's value.
		**/
		void addHeader(std::string name, std::string val);

		/**
		* @desc Clears all of the request's headers.
		**/
		void clearHeaders();

		/**
		* @desc Converts the Request object to a vector of buffers that can be sent over the network.
		* @return A vector of buffers that can be passed to Boost.Asio's write function.
		**/
		std::vector<boost::asio::const_buffer> toBuffers();

	private:
		std::forward_list<std::pair<std::string, std::string>> headers; // A list of the requests' headers
		const std::array<char, 2> crlf; // Holds the CRLF sequence
		const std::array<char, 2> nameValSep; // Contains the sequence ": "
		std::vector<boost::asio::const_buffer> bufs; // Holds the request's buffers so that they won't be deleted before the request ends
		std::vector<std::string> sdata; // Holds string data so that the buffers that refer to them won't contain garbage.
};

#endif // REQUEST_HPP
