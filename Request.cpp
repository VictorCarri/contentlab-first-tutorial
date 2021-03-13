/* Standard C++ */
#include <utility> // std::pair
#include <vector> // std::vector
#include <string> // std::string, std::string::npos
#include <ostream> // std::ostream
#include <sstream> // std::ostringstream
#include <ios> // std::hex, std::dec
#include <stdexcept> // std::invalid_argument

/* Boost */
#include <boost/asio.hpp> // boost::asio::const_buffer

/* Our headers */
#include "Request.hpp" // Class def'n

/**
* @desc Constructor. Initializes const fields and sets the animal type and number of facts to the default values.
**/
Request::Request() : crlf("\r\n"), // Initialize the line-ender string
    nameValSep(": "), // Initialize the separator for header names and values
    animalType(""), // Initialized to an empty string to represent not sending it with the request
    amount(-1), // Initialized to -1 to represent not sending it with the request
    reservedChars("!#$&'()*+,/:;=?@[]") // Reserved characters for URI-encoding
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

    //reqStream << "GET /facts/random HTTP/1.0" << crlf; // Write the request line

    /* Construct the first line of the request and add query parameters if needed */
    reqStream << "GET /facts/random"; // We always add this

    if (amount > -1 || !animalType.empty()) // We'll be sending at least 1 param, so add a query marker
    {
        reqStream << "?";
    }
    
    if (amount > -1) // Amount is set, send it with the query
    {
        reqStream << "amount=" << amount; // No need to URL-encode it because it's an integer
    }

    if (!animalType.empty()) // Animal type is set, send it as well
    {
        if (amount > -1) // Amount was set and added to the query string previously, so we need to add an "&"
        {
            reqStream << "&";
        }
    
        reqStream << "animal_type=" << uriEncode(animalType);
    }

    reqStream << " HTTP/1.0" << crlf; // End the request line, regardless of whether or not we will send parameters

    for (std::pair<std::string, std::string> header : headers) // Write each header
    {
        reqStream << header.first << nameValSep << header.second << crlf; // Write the header's name, a separator, the value, and a CRLF terminator
    }

    reqStream << crlf; // End the request
}

/**
* @desc Encodes the given string by replacing characters in the reserved set with % codes.
* @param toEncode The string to encode.
* @return The string with all reserved characters replaced with % codes using their hexadecimal values.
**/
std::string Request::uriEncode(std::string toEncode)
{
    std::ostringstream convSS; // Used to build the converted string

    for (char c : toEncode) // Check each byte in order
    {
        if (reservedChars.find(c) != std::string::npos) // This byte is a reserved character
        {
            convSS << "%" << std::hex << static_cast<unsigned short>(c) << std::dec; // Write the % and the character's value in hex
        }

        else // This character isn't a reserved character
        {
            convSS << c; // Just insert it
        }
    }

    return convSS.str(); // Return the converted string
}

/**
* @desc Sets the animal type to send to the given value.
* @param newType The new animal type to send.
**/
void Request::setAnimalType(std::string newType)
{
    animalType = newType;
}

/**
* @desc Resets the Request's parameters to default values.
**/
void Request::reset()
{
    animalType = "";
    amount = -1;
}

/**
* @desc Sets the amount to send to the given value.
* @param newAmount The new amount to send.
**/
void Request::setAmount(long long newAmount)
{
    if (newAmount >= -1 && newAmount < 500) // Validate the amount
    {
        amount = newAmount;
    }

    else // Error
    {
        throw std::invalid_argument("The amount to send must be in the range (0, 500]");
    }
}
