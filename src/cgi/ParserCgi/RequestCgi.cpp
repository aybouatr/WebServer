#include "../../../include/cgi.hpp"

    // std::string _method;
    // std::string _target;
    // std::string _querys;
    // std::string _version;
    // std::string _bodyFilePath;
    // std::map<std::string, std::string> _headers;

Cgi::Cgi(MySpace::DataRequestForGetDelete &Request, int fd_client, const Config& config)
    : Config(config), _method(Request.method), _target(Request.target), _querys(Request.queryString), 
      _version(Request.version), _bodyFilePath(""), _headers(Request.headers), _fd_client(fd_client)
      {

      }

Cgi::~Cgi() {

}

std::string Cgi::getMethod() const { return _method; }

ssize_t sendAll(int socket_fd, const char *data, size_t length)
{
    size_t total_sent = 0;
    size_t remaining = length;
    while (total_sent < length)
    {
        
        ssize_t bytes_sent = send(socket_fd, data + total_sent, remaining, 0);
        if (bytes_sent < 0)
            return -1;
        total_sent += bytes_sent;
        remaining -= bytes_sent;
    }
    return total_sent;
}
std::string Cgi::getTarget() const { return _target; }
std::string Cgi::getQuerys() const { return _querys; }
std::string Cgi::getVersion() const { return _version; }
std::string Cgi::getBodyFilePath() const { return _bodyFilePath; }

std::string Cgi::getPath() const
{
    size_t queryPos = _target.find('?');
    if (queryPos != std::string::npos)
        return _target.substr(0, queryPos);
    return _target;
}

std::string Cgi::getURI() const
{
    std::string uri = _target;
    if (!_querys.empty() && uri.find('?') == std::string::npos)
        uri = uri + "?" + _querys;
    return uri;
}

std::string Cgi::getHeader(const std::string& key) const
{
    std::string lowerKey = key;
    for (size_t i = 0; i < lowerKey.length(); ++i)
        lowerKey[i] = std::tolower(lowerKey[i]);
    
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
         it != _headers.end(); ++it)
    {
        std::string headerKey = it->first;
        for (size_t i = 0; i < headerKey.length(); ++i)
            headerKey[i] = std::tolower(headerKey[i]);
        
        if (headerKey == lowerKey)
            return it->second;
    }
    return "";
}

const std::map<std::string, std::string>& Cgi::getHeaders() const
{
    return _headers;
}

std::string Cgi::getBody() const
{
    if (_bodyFilePath.empty())
        return "";
    
    std::ifstream file(_bodyFilePath.c_str(), std::ios::binary);
    if (!file.is_open())
        return "";
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string Cgi::getFileExtension() const
{
    std::string path = getPath();
    
    size_t dotPos = path.rfind('.');
    
    if (dotPos != std::string::npos && dotPos != 0)
    {
        size_t slashAfterDot = path.find('/', dotPos);
        if (slashAfterDot == std::string::npos)
            return path.substr(dotPos);
    }
    return "";
}

void Cgi::setMethod(const std::string& method) { _method = method; }
void Cgi::setTarget(const std::string& target) { _target = target; }
void Cgi::setQuerys(const std::string& querys) { _querys = querys; }
void Cgi::setVersion(const std::string& version) { _version = version; }
void Cgi::setBodyFilePath(const std::string& filePath) { _bodyFilePath = filePath; }

void Cgi::addHeader(const std::string& key, const std::string& value)
{
    _headers[key] = value;
}

void Cgi::setHeaders(const std::map<std::string, std::string>& headers)
{
    _headers = headers;
}


void Cgi::clear()
{
    _method.clear();
    _target.clear();
    _querys.clear();
    _version.clear();
    _bodyFilePath.clear();
    _headers.clear();
}
