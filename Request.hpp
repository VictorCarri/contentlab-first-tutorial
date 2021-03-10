#ifndef REQUEST_HPP
#define REQUEST_HPP

/* Standard C++ */
#include <ostream> // std::ostream
#include <utility> // std::pair
#include <string> // std::string
#include <vector> // std::vector

/* Boost */
#include <boost/asio.hpp> // boost::asio::streambuf

class Request
{
    public:
        /**
        * @desc Constructor. Initializes const fields.
        **/
        Request();

        /**
        * @desc Adds a new header to the request.
        * @param name The header's name.
        * @param val The header's value.
        **/
        void addHeader(std::string name, std::string val);

	/**
	* @desc Returns the request's internal buffer for Boost.Asio's write() function.
	**/
	boost::asio::streambuf& getBuf();

	/**
	* @desc Tells the Request object to convert itself to a streambuf for writing.
	**/
	void createBuf();

    private:
        std::vector<std::pair<std::string, std::string>> headers; // A list of the requests' headers
        const std::string crlf; // Holds the CRLF sequence
        const std::string nameValSep; // Contains the sequence ": "
	boost::asio::streambuf reqBuf; // Buffer that holds our request
};

#endif // REQUEST_HPP
