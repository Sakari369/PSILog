#include <memory>

#include "logger.h"

int main(int argc, char *argv[]) {
	//std::unique_ptr<Logger> logger = std::make_unique<Logger>();
	Logger logger;
	logger.set_log_filter(Logger::INFO | Logger::WARN);

	std::unique_ptr<LoggerOutput> console_output = std::make_unique<LoggerConsoleOutput>();
	logger.add_output(std::move(console_output));

	logger(Logger::INFO) << "All systems initialized" << std::endl;
	logger(Logger::WARN) << "WARNING: Phasers damaged" << std::endl;
	logger(Logger::ERR) << "ERROR: Failed to boot phasers" << std::endl;

	return 0;
}
