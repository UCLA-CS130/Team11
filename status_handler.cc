// #include "status_handler.h"
// #include "http_constants.h"
// #include <sstream>

// RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config){
// 	uri_ = uri_prefix;
// 	std::cout << "Called status handler Init" << std::endl;
// 	return RequestHandler::Status::OK;
// }

// RequestHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response) {
// 	// response->SetStatus(Response::ResponseCode::OK); //TODO: make sure this is set in enum ResponseCode
// 	// response->AddHeader(CONTENT_TYPE, HTML);
	
// 	// // ~~~~~~~ code here ~~~~~~~


// 	// response->SetBody();
// 	std::cout << "Called status handler handle request" << std::endl;
// 	return RequestHandler::Status::OK;
// }