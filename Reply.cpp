/* Our headers */
#include "Reply.hpp" // Class def'n
        
/**
* @desc Fetches a reference to the buffer so that the caller can pass it to Boost.Asio.
**/
boost::asio::streambuf& Reply::buffer()
{
    return repBuf; // Return a reference
}
