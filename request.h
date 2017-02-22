#ifndef REQUEST_H
#define REQUEST_H

#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <boost/log/trivial.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>


class Request {
public:
  Request(const std::string& raw_request)  
  : raw_request_(raw_request), method_(""), 
  URI_(""), path_(""), file_(""), mime_type_(""), version_("")  { build_map(); }

  std::unique_ptr<Request> Parse(const std::string& raw_request);

  std::string raw_request() const { return raw_request_; }
  std::string method() const { return method_; }
  std::string uri() const { return URI_; }
  std::string version() const { return version_; }
  std::string path() const { return path_; }
  std::string file() const { return file_; }
  std::string mime_type() const { return mime_type_; }

  using Headers = std::vector<std::pair<std::string, std::string>>;
  Headers headers() const;

  // [TODO] Parse body of request
  // std::string body() const;

  void print_contents();

private:
  void build_map();
  std::string extension_to_type(std::string ext);

  std::map<std::string, std::string> mime_map_;
  std::string raw_request_;
  std::string method_; 
  std::string URI_;  
  std::string path_; 
  std::string file_;
  std::string mime_type_;
  std::string version_;

};

#endif