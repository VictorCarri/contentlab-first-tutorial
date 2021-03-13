#ifndef RESPONSE_HPP
#define RESPONSE_HPP

/* Standard C++ */
#include <vector> // std::vector

/* Boost */
#include <boost/asio.hpp> // boost::asio::streambuf
#include <boost/regex.hpp> // boost::regex

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

    private:
        boost::asio::streambuf repBuf; // Holds the latest data read from the socket
        short status; // The reply's status
        boost::regex headerReg; // Used to parse headers
        boost::regex statReg; // Used to parse the status line
};

#endif // RESPONSE_HPP
