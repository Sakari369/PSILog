#include <memory>

#include "make_unique.h"
#include "logger.h"

int main(int argc, char *argv[]) {
	//std::unique_ptr<Logger> logger = std::make_unique<Logger>();
	Logger logger;
	logger.set_log_level(Logger::INFO || Logger::WARN);
	logger.set_output(&std::cout);
	//logger << "All systems initialized";

	return 0;
}
