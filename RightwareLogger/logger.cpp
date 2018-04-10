#include "logger.h"

Logger::Logger() {
	std::cout << "Logger created" << std::endl;
}

Logger::~Logger() {
	std::cout << "Logger destroyed" << std::endl;
}

// Flush our output stream to our target destination
// Here we should limit the output according the current output level ?
// Actually we shouldn't even store the log messages if the filter level
// Is limiting that
void Logger::flush() {
	// Log to console for now
	std::cout << _output_stream.str();

	// Clear it
	_output_stream.str( std::string() );
	_output_stream.clear();
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
