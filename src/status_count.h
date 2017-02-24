#ifndef STATUS_COUNT_H
#define STATUS_COUNT_H

#include <map>
#include <string>

#include "request_handler.h"

// number of requests
// break down by URL requested and response code returned
// what request handlers exist and for what URL prefixes?

class StatusCount {
  public:
	StatusCount(StatusCount const&) = delete;
	void operator=(StatusCount const&) = delete;
	static StatusCount& get_instance();

  	std::map<std::string, std::map<int,int>>statuses_map_;
  	std::map<std::string, std::string>handlers_map_;
  	int request_count_ = 0;
  private:
  	StatusCount();
};

#endif // STATUS_COUNT_H