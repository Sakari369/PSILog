#include "logger.h"

Logger::Logger() {
	std::cout << "Logger created" << std::endl;
}

Logger::~Logger() {
	std::cout << "Logger destroyed" << std::endl;
}

void Logger::set_log_level(int log_level) {
	_log_level = log_level;
}

int Logger::get_log_level() {
	return _log_level;
}

void Logger::set_output(std::ostream *output) {
	_output = output;
}
