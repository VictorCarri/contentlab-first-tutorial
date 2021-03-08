/* C++ versions of C headers */
#include <cstddef> // std::size_t
#include <ctime> // std::time_t, std::time, std::tm, strftime

/* Standard C++ headers */
#include <iostream> // std::clog, std::endl
#include <vector> // std::vector

/* Boost */
#include <boost/asio.hpp> // All boost::asio includes
#include <boost/system/error_code.hpp> // boost::system::error_code

/* Our headers */
#include "Request.hpp" // Represents a request

/* Defines */
#define DATE_SIZE 128 // Max date size length

int main()
{
    boost::asio::io_context ioc; // For all I/O ops
    boost::asio::ip::tcp::resolver resolver(ioc); // To convert a hostname to a list of endpoints
    auto resolveRes = resolver.resolve("cat-fact.herokuapp.com", "80"); // Get a list of HTTP endpoints

    #ifdef DEBUG
    int eNo = 1;
    for (boost::asio::ip::tcp::endpoint endPoint : resolveRes)
    {
        std::clog << "Endpoint #" << eNo << std::endl
        << "\tAddress: " << endPoint.address().to_string() << std::endl
        << "\tCapacity: " << endPoint.capacity() << std::endl
        << "\tPort: " << endPoint.port() << std::endl
        << "\tSize: " << endPoint.size() << std::endl
        << std::endl;
        ++eNo;
    }
    #endif

    boost::asio::ip::tcp::socket sock(ioc); // THe socket over which we communicate with the server
    #ifdef DEBUG
    auto connectedEndpoint = boost::asio::connect(sock, resolveRes); // Connect to an endpoint
    #else
    boost::asio::connect(sock, resolveRes); // Connect to an endpoint
    #endif

    #ifdef DEBUG
    /* Print endpoint details to prove that it connected to one */
    std::clog << "Connected to an endpoint." << std::endl
    << "\tAddress: " << connectedEndpoint.address().to_string() << std::endl
    << "\tCapacity: " << connectedEndpoint.capacity() << std::endl
    << "\tPort: " << connectedEndpoint.port() << std::endl
    << "\tSize: " << connectedEndpoint.size() << std::endl;
    #endif

    /* Create a Request and convert it to buffers, then send it */
    Request req; // Just default-construct it for now
    req.addHeader("Accept", "application/json"); // Request a JSON response from the API
    req.addHeader("Accept-Charset", "utf-8"); // Most common charset on the Internet
    req.addHeader("Content-Length", "0"); // No body
    std::time_t now = std::time(NULL); // Get the current time
    std::tm* localTime = std::localtime(&now); // Convert it to calendar time
    char timeStr[DATE_SIZE]; // Time string

    #ifdef DEBUG
    std::size_t bytesWritten = std::strftime(timeStr, 64, "%a, %d %b %Y %H:%M:%S %Z", localTime);
    std::cout << "Date: " << timeStr << std::endl
    << "Date size: " << bytesWritten << std::endl;
    #else
    std::strftime(timeStr, 64, "%a, %d %b %Y %H:%M:%S %Z", localTime);
    #endif

    req.addHeader("Date", timeStr);
    req.addHeader("Host", "cat-fact.herokuapp.com"); // Mandatory header
    req.addHeader("User-Agent", "Cat Fact Client/1.0"); // For fun
    std::vector<boost::asio::const_buffer> reqBufs = req.toBuffers(); // Convert the request to buffers

    #ifdef DEBUG
    std::clog << "Request buffers: " << std::endl;
    int bufNo = 1;

    for (boost::asio::const_buffer buf : reqBufs)
    {
        std::clog << bufNo << ")\t";
        const char* bufDat = static_cast<const char*>(buf.data()); // Get the buffer's data
        std::size_t bufSiz = buf.size(); // Get the size of the buffer's data

        for (std::size_t i = 0; i < bufSiz; i++) // Print each character
        {
            std::clog << bufDat[i];
        }

        std::clog << std::endl; // End the line
        ++bufNo;
    }
    #endif

    boost::asio::write(sock, reqBufs); // Send our request to the server

    #ifdef DEBUG
    std::clog << "Sent request to the server!" << std::endl;
    #endif

    return 0;
}
