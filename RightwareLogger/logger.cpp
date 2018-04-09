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

int Logger::get_log_level() const {
	return _log_level;
}

int Logger::get_log_filter() const {
	return _log_filter;
}

void Logger::set_log_filter(int log_filter) {
	_log_filter = log_filter;
}

// Flush our output stream to our target destination
void Logger::flush() {
	// Log to console for now
	std::cout << _output_stream.str();

	// Clear it
	_output_stream.str( std::string() );
	_output_stream.clear();
}
