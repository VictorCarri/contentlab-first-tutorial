/* Standard C++ */
#include <istream> // std::istream
#include <string> // std::string, std::getline
#ifdef DEBUG
#include <iostream> // std::clog
#include <iomanip> // std::quoted
#endif
#include <sstream> // std::stringstream
#include <utility> // std::make_pair

/* Boost */
#include <boost/asio.hpp> // boost::asio::streambuf
#include <boost/regex.hpp> // boost::regex_match, boost::smatch
#include <boost/any.hpp> // boost::any

/* Our headers */
#include "Reply.hpp" // Class def'n
        
/**
* @desc Fetches a reference to the buffer so that the caller can pass it to Boost.Asio.
**/
boost::asio::streambuf& Reply::buffer()
{
    return repBuf; // Return a reference
}

/**
* @desc Parses the contents of the buffer as the status line of an HTTP response.
* @return True if we successfully parsed it, false otherwise.
**/
bool Reply::parseStatusLine()
{
	std::istream bufStrm(&repBuf); // Construct an istream that will read data from the buffer
	std::string statusLine;
	boost::smatch what; // What matched

	std::getline(bufStrm, statusLine, '\r'); // Read until the CR
	bufStrm.get(); // Read the NL so that the first header will be read properly
	
	if (boost::regex_match(statusLine, what, statReg)) // This is a valid status line
	{
		#ifdef DEBUG
		for (unsigned long long i = 0; i < what.size(); i++) // Print each match
		{
			std::clog << "Match #" << (i+1) << ": \"" << what[i] << "\"" << std::endl;
		}
		#endif

		std::string code = what[1].str(); // The status code
		std::istringstream statStrm(code); // Construct a stream to read it as a short
		statStrm >> status; // Read the status
		#ifdef DEBUG
		std::cout << "Reply::parseStatusLine: status code = " << status << std::endl;
		#endif
		return (status == 200);
	}

	else
	{
		return false;
	}
}

/**
* @desc Fetches the reply's status.
* @return The reply's status.
**/
short Reply::getStatus() const
{
	return status;
}

/**
* @desc Determines whether the data in the buffer is just the string "\r\n" - the final terminator.
* @return True if the data in the buffer is only "\r\n", false otherwise.
**/
bool Reply::isFinalTerminator()
{
	std::istream bufStrm(&repBuf);
	bool toReturn = false;
	
	#ifdef DEBUG
	std::cout << "Reply::isFinalTerminator: bufStrm.peek() == '" << bufStrm.peek() << "'%" << std::endl;
	#endif
	
	if (bufStrm.peek() == '\r') // Current character is "\r", check if the next is "\n"
	{
		bufStrm.get(); // Read the "\r"

		#ifdef DEBUG
		std::cout << "Reply::isFinalTerminator: inside outer if: bufStrm.peek() == '" << bufStrm.peek() << "'%" << std::endl;
		#endif
	
		if (bufStrm.peek() == '\n') // Next character is the line-term, so this is the final terminator
		{
			toReturn = true;
		}

		else // Not the final terminator
		{
			bufStrm.unget(); // Undo the extraction, so that the buffer's contents will still be valid
		}
	}

	return toReturn; // Will be false unless we read "\r\n"
}

/**
* @desc Parses the contents of the buffer as a header and stores the header value.
* @return True if the contents of the buffer were a valid header, false otherwise.
**/
bool Reply::parseHeader()
{
	std::istream bufStrm(&repBuf); // Construct an istream that will read data from the buffer
	std::string headerLine; // Will hold the entire header line
	bool toReturn = false;

	std::getline(bufStrm, headerLine, '\r'); // Read until the CR
	bufStrm.get(); // Clear the "\n" 
	#ifdef DEBUG
	std::clog << "Reply::parseHeader: header line = " << std::quoted(headerLine) << std::endl;
	#endif

	boost::smatch what; // What matched	

	if (boost::regex_match(headerLine, what, headerReg)) // Valid header line
	{
		std::string headerName = what[1].str();

		#ifdef DEBUG
		std::cout << "Reply::parseHeader: header line is valid." << std::endl
		<< "Header name: \"" << headerName << "\"" << std::endl;
		#endif

		if (headerName == "Content-Length") // We need to parse the value as an integer
		{
			std::string lengthStr = what[2].str();

			#ifdef DEBUG
			std::cout << "Length of JSON as a string: " << std::quoted(lengthStr) << std::endl;
			#endif

			std::istringstream lengthStrm(lengthStr); // To convert it to an integer
			lengthStrm >> length; // Read the length

			#ifdef DEBUG
			std::cout << "Length of JSON as an integer: " << length << std::endl;
			#endif

			headers.emplace_back(headerName, length);
		}

		else // We can just store the value as a string
		{
			headers.emplace_back(headerName, what[2].str());
		}

		toReturn = true;
	}

	else // Invalid header line
	{
		#ifdef DEBUG
		std::cout << "Reply::parseHeader: header line is invalid." << std::endl;
		#endif
	}

	return toReturn;
}

/**
* @desc Constructor. Initializes regexes.
**/
Reply::Reply() : headerReg("^(.*): (.*)$"), // Parse the name and contents of a header separately
	statReg("^HTTP/[0-9]\\.[0-9] ([0-9]{3}) (.*)$") // Match a status line according to the RFC - HTTP/{versionMajor}.{versionMinor} (responseCode) responseText
{
}

/**
* @desc Fetches the response's length.
* @return The response's length.
**/
std::size_t Reply::getLength() const
{
	return length;
}
