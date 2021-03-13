#ifndef RESPONSE_HPP
#define RESPONSE_HPP

/* C++ versions of C headers */
#include <cstddef> // std::size_t

/* Standard C++ */
#include <vector> // std::vector
#include <utility> // std::pair
#include <string> // std::string

/* Boost */
#include <boost/asio.hpp> // boost::asio::streambuf
#include <boost/regex.hpp> // boost::regex
#include <boost/any.hpp> // boost::any

class Reply
{
    public:
        /**
        * @desc Fetches a reference to the buffer so that the caller can pass it to Boost.Asio.
        **/
        boost::asio::streambuf& buffer();

        /**
        * @desc Parses the contents of the buffer as the status line of an HTTP response.
	* @return True if we successfully parsed it, false otherwise.
        **/
        bool parseStatusLine();

        /**
        * @desc Fetches the reply's status.
        * @return The reply's status.
        **/
        short getStatus() const;

        /**
        * @desc Determines whether the data in the buffer is just the string "\r\n" - the final terminator.
        * @return True if the data in the buffer is only "\r\n", false otherwise.
        **/
        bool isFinalTerminator(); 

        /**
        * @desc Parses the contents of the buffer as a header and stores the header value.
	* @return True if the contents of the buffer were a valid header, false otherwise.
        **/
        bool parseHeader();

        /**
        * @desc Constructor. Initializes regexes.
        **/
        Reply();

	/**
	* @desc Fetches the response's length.
	* @return The response's length.
	**/
	std::size_t getLength() const;

    private:
        boost::asio::streambuf repBuf; // Holds the latest data read from the socket
        short status; // The reply's status
        boost::regex headerReg; // Used to parse headers
        boost::regex statReg; // Used to parse the status line
	std::vector<std::pair<std::string, boost::any>> headers; // The response's headers
	std::size_t length; // Content length
};

#endif // RESPONSE_HPP
