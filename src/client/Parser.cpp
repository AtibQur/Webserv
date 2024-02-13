#include "../../inc/main.hpp"

/* Parse the client request */

std::string subTillChar(std::string str, size_t index, char c)
{
    std::string tmp = "";
    size_t pos1 = str.find('\r', index);
    size_t pos2 = str.find(c, index);
    if (pos1 != std::string::npos || pos2 != std::string::npos)
    {
        if (pos1 < pos2)
        {
            tmp = str.substr(index, pos1 - index);
        }
        else
        {
            tmp = str.substr(index, pos2 - index);
        }
    }
    return tmp;
}

void Client::checkBytesInFile()
{
    std::ifstream in_file("root/body.txt", std::ios::binary);
    in_file.seekg(0, std::ios::end);
    int file_size = in_file.tellg();
    std::cout << "Size of the file is " << file_size << " bytes";
}

void ptn(std::string str)
{
    std::cout << str << std::endl;
    int i = 0;
    while (str[i])
    {
        if (str[i] == '\r')
            std::cout << "r";
        else if (str[i] == '\n')
            std::cout << "n";
        else
            std::cout << str[i];
        i++;
        break;
    }
}

int Client::parseRequest(std::string request, ssize_t post)
{
    std::stringstream httpRequest(request);
    std::string tmp;

    if (!checkRequestLine(request))
    {
        throw std::invalid_argument("400 Bad Request");
    }
    getline(httpRequest, tmp, ' ');
    if (!checkMethod(tmp))
    {
        throw std::invalid_argument("405 Method not Allowed");
    }
    _method = tmp;
    getline(httpRequest, tmp, ' ');

    if (tmp.empty())
    {
        throw std::invalid_argument("400 Bad Request");
    }
    _uri = tmp;
    getline(httpRequest, tmp);

    if (tmp.compare("HTTP/1.1\r")) // \r\n
        throw std::invalid_argument("400 Bad Request");
    _protocol = tmp;

    // start header
    if (request.find("\r\n\r\n") == std::string::npos)
    {
        std::cout << "the request is not complete" << std::endl;
        return 1;
    }
    if (_method == "DELETE") 
    {
        _fileNameBody = _uri;
        return 0;
    }
    else if (_method == "GET")
    {
        return 0;
    }

    // parse header
    while (getline(httpRequest, tmp))
    {
        // save name
        if (_contentType == "text/plain" && tmp.find("post=") != std::string::npos)
        {
            m_name = tmp.substr(5);
        }
        if (tmp.find("--" + _boundary) != std::string::npos)
            break;
        if (tmp.find("Content-Type:") != std::string::npos)
        {
            _contentType = subTillChar(tmp, tmp.find("Content-Type:") + 14, ';');
            _boundary = tmp.substr(tmp.find("boundary") + 9);
        }
        if (tmp.find("Content-Length:") != std::string::npos)
        {
            _contentLength = stoll(tmp.substr(tmp.find("Content-Length:") + 16));
        }
    }
    if (_contentType != "multipart/form-data" || _contentType.empty())
    {
        return (0); // for when its text or www-form-urlencoded
    }
    if (_contentLength == 0)
    {
        throw std::invalid_argument("400 Bad Request: Content-Length is 0"); // not always true ( need to check for data type before error checking )
    }
    if (_contentType.empty())
    {
        throw std::invalid_argument("400 Bad Request: Content-Type is empty"); // not always true
    }
    if (_boundary.empty())
        throw std::invalid_argument("400 Bad Request: Boundary is empty");
    if (_contentLength > _maxBodySize)
    { // needs to be updated from conf file
        throw std::invalid_argument("413 Payload Too Large");
    }

    // parse body
    getline(httpRequest, tmp);
    if (tmp.find("name=") != std::string::npos)
    {
        if (subTillChar(tmp, tmp.find("name=") + 6, '\"') == "delete")
        {
            _method = "DELETE";
            _isDelete = true;
        }
    }

    if (tmp.find("filename=") != std::string::npos)
    {
        _fileNameBody = subTillChar(tmp, tmp.find("filename=") + 10, '\"');
        _fileNameBody = decodePercentEncoding(_fileNameBody); // Decode filename
        std::cout << "File name body: " << _fileNameBody << std::endl;
        if (checkForSpaces(_fileNameBody))
        {
            _fileNameBody = urlEncode(_fileNameBody); // Encode spaces

            transferData();
        }
    }
    getline(httpRequest, tmp);
    if (tmp.find("Content-Type:") != std::string::npos)
        _contentType = subTillChar(tmp, tmp.find("Content-Type:") + 14, '\r');
    if (_method == "DELETE")
        return (0);
    getline(httpRequest, tmp);
    
    while (getline(httpRequest, tmp)) {
        if (_boundary.find(tmp) != std::string::npos && _contentType == "text/plain") {
            break ;
        }
        _body.append(tmp);
        _body.append("\n");
    }
    std::stringstream ss(_body);
    std::string read;
    std::ofstream bodyfile;

    Location location = _location[_uri];

    bodyfile.open("./root/" + _fileNameBody);
    if (!bodyfile)
    {
        return 0;
    }
    while (getline(ss, read, '\n'))
    {
        bodyfile << read;
        bodyfile << std::endl;
    }

    bodyfile.close();

    // response zin eindigt met /r/n
    // hele response eidigt met /r/n/r/n
    // content length bepaalt of the body compleet is (als er een body is)

    return (0);
}

int Client::transferData()
{
    std::string decodedFileName = "./root/" + decodePercentEncoding(_fileNameBody);
    std::string encodedFileName = "./root/" + urlEncode(_fileNameBody);

    // Open the source file for reading
    std::ifstream sourceFile(decodedFileName);
    if (!sourceFile.is_open())
    {
        std::cerr << "Error opening source file: " << decodedFileName << std::endl;
        return 1;
    }

    // Open the destination file for writing
    std::ofstream destinationFile(encodedFileName);
    if (!destinationFile.is_open())
    {
        std::cerr << "Error opening destination file: " << encodedFileName << std::endl;
        return 1;
    }

    // Transfer data from source file to destination file
    char ch;
    while (sourceFile.get(ch))
    {
        destinationFile.put(ch);
    }

    // Close the files
    sourceFile.close();
    destinationFile.close();

    return 0;
}

std::string Client::decodePercentEncoding(const std::string &encoded)
{
    std::string decoded{};

    for (size_t i = 0; i < encoded.length(); ++i)
    {
        if (encoded[i] == '%' && i + 2 < encoded.length())
        {
            std::string hexValue = encoded.substr(i + 1, 2);
            decoded += hexToChar(hexValue);
            i += 2;
        }
        else
            decoded += encoded[i];
    }

    return decoded;
}

std::string Client::urlEncode(const std::string &input)
{
    std::ostringstream encoded;

    for (char c : input)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            encoded << c;
        else
            encoded << '%' << std::uppercase << std::hex << ((c >> 4) & 0x0F) << (c & 0x0F);
    }

    return encoded.str();
}

char Client::hexToChar(const std::string &hex)
{
    int value{};
    std::stringstream ss{};

    ss << std::hex << hex;
    ss >> value;

    return static_cast<char>(value);
}

bool Client::checkForSpaces(std::string fileNameBody)
{
    for (size_t i = 0; i < fileNameBody.size(); i++)
    {
        if (std::isspace(fileNameBody[i]))
        {
            return true;
        }
    }
    return false;
}

// check if the requestline is valid
bool Client::checkRequestLine(std::string httpRequest)
{
    size_t space1 = httpRequest.find(' ');
    size_t space2 = httpRequest.find(' ', space1 + 1);

    if (space1 == std::string::npos || space2 == std::string::npos)
        return false;
    _method = httpRequest.substr(0, space1);
    _uri = httpRequest.substr(space1 + 1, space2 - space1 - 1);
    _protocol = httpRequest.substr(space2 + 1);
    return true;
}

// check if the method is valid
bool Client::checkMethod(std::string tmp)
{
    static const std::string validMethods[3] = {"GET", "POST", "DELETE"};
    for (int i = 0; i < 3; i++)
    {
        if (tmp == validMethods[i])
            return true;
    }
    return false;
}
