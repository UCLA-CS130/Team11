#include "status_count.h"

StatusCount::StatusCount() {}

StatusCount& StatusCount::get_instance() {
	static StatusCount instance;
	return instance;
}
