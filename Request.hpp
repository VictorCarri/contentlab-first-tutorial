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
        * @desc Constructor. Initializes const fields and sets the animal type and number of facts to the default values.
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
    
        /**
        * @desc Sets the animal type to send to the given value.
        * @param newType The new animal type to send.
        **/
        void setAnimalType(std::string newType);
    
        /**
        * @desc Resets the Request's parameters to default values.
        **/
        void reset();
    
        /**
        * @desc Sets the amount to send to the given value.
        * @param newAmount The new amount to send.
        **/
        void setAmount(long long newAmount);
    
    private:
        /**
        * @desc Encodes the given string by replacing characters in the reserved set with % codes.
        * @param toEncode The string to encode.
        * @return The string with all reserved characters replaced with % codes using their hexadecimal values.
        **/
        std::string uriEncode(std::string toEncode);

        std::vector<std::pair<std::string, std::string>> headers; // A list of the requests' headers
        const std::string crlf; // Holds the CRLF sequence
        const std::string nameValSep; // Contains the sequence ": "
        boost::asio::streambuf reqBuf; // Buffer that holds our request
        std::string animalType; // Type of animal to request facts about. Defaults to the empty string to represent not sending it with the request.
        long long amount; // Defaults to -1 to represent not sending it with the request.
        std::string reservedChars; // Characters that we need to URI-encode
};

#endif // REQUEST_HPP
