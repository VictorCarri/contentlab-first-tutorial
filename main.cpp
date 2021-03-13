/* C++ versions of C headers */
#include <cstddef> // std::size_t

/* Standard C++ headers */
#include <iostream> // std::clog, std::endl
#include <vector> // std::vector
#include <string> // std::string
#include <sstream> // std::ostringstream
#include <exception> // std::exception
#include <string> // std::string
#include <memory> // std::unique_ptr
#ifdef DEBUG
#include <iomanip> // std::quoted
#endif

/* Boost */
#include <boost/asio.hpp> // All boost::asio types
#include <boost/system/error_code.hpp> // boost::system::error_code
#include <boost/json.hpp> // boost::json::value, boost::json::parse
#include <boost/program_options.hpp> // boost::program_options::options_description, boost::program_options::value, boost::program_options::variables_map, boost::program_options::store, boost::program_options::notify, boost::program_options::error, boost::program_options::command_line_parser

/* Our headers */
#include "Request.hpp" // Represents a request
#include "Reply.hpp" // Represents a reply from the server

/* Defines */
#define DATE_SIZE 128 // Max date size length

/**
* @desc Converts buffers to a string for debugging.
* @param bufs The buffers to dump.
* @param bytesRead # of bytes read so far.
* @return The contents of the buffers as a string.
**/
std::string bufsToStr(boost::asio::streambuf::const_buffers_type bufs, std::size_t bytesRead)
{
        std::ostringstream dataSS; // We will insert all of the buffers' data into this to convert it to a single string for parsing/printing
        std::size_t bytesInserted = 0; // # of bytes inserted so far. Used to ensure that we don't read more data than Boost.Asio read
    
        for (boost::asio::const_buffer buf : bufs) // Check each buffer that read_until stored for data
        {
            const char* curBufDat = static_cast<const char*>(buf.data()); // Fetch this buffer's data as a C string
            std::size_t bufSiz = buf.size(); // Length of this buffer's data
    
            for (std::size_t i = 0; i < bufSiz; i++) // Avoid buffer overrun
            {
                if (bytesInserted < bytesRead) // Ensure that we don't insert more bytes than read_until actually read
                {
                    dataSS << curBufDat[i]; // Insert this byte into the stringstream
                    ++bytesInserted; // Ensure that we'll stop inserting more characters as soon as we read the correct # of bytes
                }
            }
        }

        return dataSS.str();
}

/**
* @desc Converts buffers to a string for debugging.
* @param bufs The buffers to dump.
* @return The contents of the buffers as a string.
**/
std::string bufsToStr(boost::asio::streambuf::const_buffers_type bufs)
{
        std::ostringstream dataSS; // We will insert all of the buffers' data into this to convert it to a single string for parsing/printing
    
        for (boost::asio::const_buffer buf : bufs) // Check each buffer that read_until stored for data
        {
            const char* curBufDat = static_cast<const char*>(buf.data()); // Fetch this buffer's data as a C string
            std::size_t bufSiz = buf.size(); // Length of this buffer's data
    
            for (std::size_t i = 0; i < bufSiz; i++) // Avoid buffer overrun
            {
                dataSS << curBufDat[i]; // Insert this byte into the stringstream
            }
        }

        return dataSS.str();
}

enum ExitCode
{
    NORMAL = 0,
    PARSING_ERR,
    HELP,
    MAX_ERR
};

const char* getFactText(const boost::json::value& val)
{
    const boost::json::object& factObj = val.as_object(); // Fetch the value as an object
    const boost::json::value& factText = factObj.at("text"); // Fetch the text of the cat fact as a value
    const boost::json::string& factStr = factText.as_string(); // Fetch the fact text as a JSON string object
    const char* factCStr = factStr.c_str(); // Fetch a pointer to the underlying character array
    return factCStr;
}

int main(int argc, char* argv[])
{
    /* Set up program arguments */
    boost::program_options::options_description desc("Options");
    std::string type;
    long long amount;
    desc.add_options()
        ("help,h", "Print this help message")
        ("type,t", boost::program_options::value<std::string>(&type)->default_value(""), "Type of animal to request facts about. \"\" if not specified.")
        ("amount,a", boost::program_options::value<long long>(&amount)->default_value(-1), "Number of facts to request (limit 500)");
    boost::program_options::variables_map vm;

    try
    {
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vm);
        boost::program_options::notify(vm);
    }

    catch (boost::program_options::error& optErr)
    {
        std::cerr << "Error while parsing arguments: " << optErr.what() << std::endl;
        return PARSING_ERR;
    }

    if (vm.count("help")) // Print help
    {
        std::cout << desc << std::endl;
        return HELP;
    }

    if (amount > 500) // API error
    {
        std::cerr << "Max number of facts is 500" << std::endl;
        return MAX_ERR;
    }

    boost::asio::io_context ioc; // For all I/O ops
    boost::asio::ip::tcp::resolver resolver(ioc); // To convert a hostname to a list of endpoints
    std::string hostName("cat-fact.herokuapp.com"); // Name of the host to connect to
    boost::asio::ip::tcp::resolver::results_type resolveRes = resolver.resolve(hostName, "http"); // Get a list of HTTP endpoints

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
    boost::asio::ip::tcp::endpoint connectedEndpoint = boost::asio::connect(sock, resolveRes); // Connect to an endpoint
    /* Print endpoint details to prove that it connected to one */
    std::clog << "Connected to an endpoint." << std::endl
    << "\tAddress: " << connectedEndpoint.address().to_string() << std::endl
    << "\tCapacity: " << connectedEndpoint.capacity() << std::endl
    << "\tPort: " << connectedEndpoint.port() << std::endl
    << "\tSize: " << connectedEndpoint.size() << std::endl;
    #else
    boost::asio::connect(sock, resolveRes); // Connect to an endpoint
    #endif

    /* Create a Request and convert it to buffers, then send it. */
    Request req; // Initialize by default construction

    /* Set parameters. Their values have already been validated. If no arguments were given, the parameters will have their default values, meaning that Request won't encode them in the URI. */
    req.setAnimalType(type);
    req.setAmount(amount);

    req.addHeader("Host", hostName); // Mandatory header
    req.addHeader("Accept", "*/*");
    req.addHeader("User-Agent", "Cat Fact Client/3.0 (Linux x86_64)");
    req.addHeader("Connection", "close");
    req.createBuf(); // Tell the Request object to create its buffer for sending the request over the network
    boost::asio::streambuf& reqBuf = req.getBuf();

    #ifdef DEBUG
    boost::asio::streambuf::const_buffers_type bufs = reqBuf.data();
    std::string reqStr = bufsToStr(bufs);
    std::cout << "Request as a string: " << std::endl
    << reqStr << std::endl;
    #endif

    boost::asio::write(sock, reqBuf); // Send our request to the server

    #ifdef DEBUG
    std::clog << "Sent request to the server!" << std::endl;
    #endif

    /** Read the response **/
    Reply rep; // Will parse the response and store data inside itself

    /* Read and parse the status line */
    boost::asio::read_until(sock, rep.buffer(), "\r\n"); // Read the status line

    if (rep.parseStatusLine())
    {
        #ifdef DEBUG
        std::cout << "Reply's status line is valid" << std::endl;
        #endif

        if (rep.getStatus() == 200) // Received a good response
        {
            /* Parse headers until we reach the terminator between the headers and the body */
            while (!rep.isFinalTerminator())
            {
                boost::asio::read_until(sock, rep.buffer(), "\r\n"); // Read the next line
                rep.parseHeader(); // Tell the reply to parse this line as a header and store its contents
                #ifdef DEBUG
                std::cout << std::endl; // Make debugging output look nicer
                #endif
            }

            /* Read until the final '}' in the response, then ensure that we read as many characters as we expected */
            std::size_t jsonLength = rep.getLength(); // # of characters to read
            std::size_t numCharsRead = 0;
            std::string initBufConts = bufsToStr(rep.buffer().data()); // First part of the JSON response
            boost::system::error_code ec; // Holds an error code. Used to check if we've read all the data
            numCharsRead += initBufConts.length(); // Count the # of characters read
        
            #ifdef DEBUG
            std::cout << "Data remaining in buffer: " << std::quoted(initBufConts) << std::endl;
            #endif

            while (numCharsRead < jsonLength)
            {
                std::size_t charsReadOnCurCall = boost::asio::read(sock, rep.buffer(), ec); // Read data and increment the # of characters read
                #ifdef DEBUG
                std::clog << "main: read " << charsReadOnCurCall << " chars on current call" << std::endl;
                #endif
                numCharsRead += charsReadOnCurCall;
                #ifdef DEBUG
                std::cout << "main: read " << numCharsRead << "/" << jsonLength << " chars of response body" << std::endl;
                #endif
            }
        
            std::string fullJSON = bufsToStr(rep.buffer().data()); // Second part of the JSON response

            #ifdef DEBUG
            std::cout << "Second part of response: " << fullJSON << std::endl;
            #endif

            boost::json::value catFact;

            try
            {
                catFact = boost::json::parse(fullJSON); // Need to use the "text" key of each fact
                #ifdef DEBUG
                std::clog << "Parsed JSON: " << std::endl
                << catFact << std::endl;
                #endif

                if (catFact.kind() == boost::json::kind::array) // Array of facts
                {
                    boost::json::array& factArr = catFact.as_array(); // Fetch the value as an array
                    int factNum = 1;
        
                    for (const boost::json::value& val : factArr) // Loop through each value in the array
                    {
                        std::cout << factNum << ")\t" << getFactText(val) << std::endl;
                        ++factNum;
                    }
                }

                else // Single object
                {
                    std::cout << getFactText(catFact) << std::endl; // Print the fact
                }
    
                return NORMAL;
            }
        
            catch (boost::system::system_error& parseErr)
            {
                std::cerr << "Error occurred while attempting to parse JSON response." << std::endl
                << parseErr.what()
                << std::endl;
            }
        }

        else // Received a bad response
        {
            std::cerr << "Error: received a bad response: " << rep.getStatus() << std::endl;
        }
    }

    else
    {
        std::cerr << "Bad status line" << std::endl;
    }

    return NORMAL;
}
