#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <map>

struct Header {
	std::string name;
  std::string value;
};

struct Response {
	enum status_type
  {
    ok = 200,
    created = 201,
    accepted = 202,
    no_content = 204,
    multiple_choices = 300,
    moved_permanently = 301,
    moved_temporarily = 302,
    not_modified = 304,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503
  } status;

  std::string name_value_separator = ": ";
  std::string crlf = "\r\n";
  std::vector<Header> headers;
	std::string header_builder();

};