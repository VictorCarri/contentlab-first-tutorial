#ifndef RESPONSE_HPP
#define RESPONSE_HPP

/* Standard C++ */
#include <vector> // std::vector

/* Boost */
#include <boost/asio.hpp> // boost::asio::streambuf

class Reply
{
    public:
        /**
        * @desc Fetches a reference to the buffer so that the caller can pass it to Boost.Asio.
        **/
        boost::asio::streambuf& buffer();

    private:
        boost::asio::streambuf repBuf; // Holds the latest data read from the socket
};

#endif // RESPONSE_HPP
