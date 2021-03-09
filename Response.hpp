#ifndef RESPONSE_HPP
#define RESPONSE_HPP

/* Standard C++ */
#include <vector> // std::vector

/* Boost */
#include <boost/asio.hpp> // boost::asio::mutable_buffer

class Response
{
    public:
        /**
        * @desc Returns a pointer to the vector of buffers so that Boost.Asio can store data into it.
        * @return A pointer to the vector of buffers.
        **/
        std::vector<boost::asio::mutable_buffer getBufs() const;

    private:
        std::vector<boost::asio::mutable_buffer> repBufs;
};

#endif // RESPONSE_HPP
