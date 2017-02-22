#include "request.h"
#include "http_constants.h"

// Helper functions
void Request::build_map() { 
  mime_map_[".gif"] = GIF;
  mime_map_[".htm"] = HTML;
  mime_map_[".html"] = HTML;
  mime_map_[".jpg"] = JPG;
  mime_map_[".jpeg"] = JPG;
  mime_map_[".png"] = PNG;
}

std::string Request::extension_to_type(std::string ext) {
  auto it = mime_map_.find(ext); 
  if (it != mime_map_.end()) {
    return it->second;
  }
  return "text/plain";
}

void Request::print_contents() {
  std::cout << "Request contains:" << std::endl;
  std::cout << "request:\n" << this->raw_request() << std::endl;
  std::cout << "METHOD: " << this->method() << std::endl;
  std::cout << "URI: " << this->uri() << std::endl;
  std::cout << "http version: " << this->version() << std::endl;
  std::cout << "path: " << this->path() << std::endl;
  std::cout << "file: " << this->file() << std::endl;
  std::cout << "mime_type: " << this->mime_type() << std::endl;  
} 

std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
  
  std::unique_ptr<Request> parsed_request(new Request(raw_request));

   // Extract request into lines:
  std::string request(raw_request); 
  std::vector<std::string> lines;
  boost::split(lines,request,boost::is_any_of("\r\n"));

  // Extract request line: 
  std::vector<std::string> reqs; 
  boost::split(reqs,lines[0],boost::is_any_of(" "));

   if (reqs.size() != 3) {
    return nullptr;
  }

  parsed_request->method_ = reqs[0];
  parsed_request->URI_ = reqs[1];
  parsed_request->version_ = reqs[2];

  // Extract file and path:
  boost::filesystem::path p(parsed_request->URI_);
  parsed_request->path_ = p.parent_path().string(); 
  parsed_request->file_ = p.filename().string();
  std::string ext = p.extension().string();
  parsed_request->mime_type_ = extension_to_type(ext); 

  return parsed_request;
}