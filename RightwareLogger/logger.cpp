#include <assert.h>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <thread>
#include <mutex>

#include "logger.h"

Logger::Logger() {
	//std::cout << "Logger created" << std::endl;
}

Logger::~Logger() {
	//std::cout << "Logger destroyed" << std::endl;
}

LogStream Logger::operator ()() {
	return LogStream(*this, LogLevel::INFO);
}

LogStream Logger::operator ()(int log_level) {
	return LogStream(*this, log_level);
}

/*
void Logger::log(const std::string &entry) {
	// If the log output hasn't been flushed to the destination yet, append
	// log entry prefix to the stream
	if (_flushed == true) {
		std::string prefix = get_log_entry_prefix(entry);
		std::thread::id this_id = std::this_thread::get_id();
		_log_stream << prefix << " " << this_id << " ";
		_flushed = false;
	}

	_log_stream << entry;
	std::cout << _log_stream.str();
	for (const auto &outputter : _outputters) {
		outputter->write_log_entry(_log_stream.str(), _log_level);
	}
}
*/

void Logger::log(const std::string &entry, int log_level) {
	std::stringstream entry_ss;
	std::string prefix = get_log_entry_prefix(entry);
	std::thread::id thread_id = std::this_thread::get_id();

	entry_ss << prefix << "[" << thread_id << "] " << entry;
	for (const auto &outputter : _outputters) {
		outputter->write_log_entry(entry_ss.str(), log_level);
	}
}

// Flush our log stream to our target output destinations
/*
void Logger::flush() {
	// Dispatch the log entry to all of our outputters
	// This operation will make sure the message is flushed also to the destination stream also
	for (const auto &outputter : _outputters) {
		outputter->write_log_entry(_log_stream.str(), _log_level);
	}

	// Clear our output stream after dispatching
	_log_stream.str( std::string() );
	_log_stream.clear();
	_flushed = true;
}
*/

std::string Logger::get_log_entry_prefix(const std::string &log_entry) const {
	std::string formatted;
	std::stringstream ss;

	// Append time to the entry
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	ss << std::put_time(&tm, "[%H:%M:%S] ");

	return ss.str();
}

// Add output destination to our chain of outputs
// TODO: do we use return value ?
bool Logger::add_output(std::unique_ptr<LoggerOutput> output) {
	assert(output != nullptr);
	_outputters.push_back(std::move(output));

	return true;
}

// Set the currently active log level for logging messages
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

// Default console output implementation
LoggerConsoleOutput::LoggerConsoleOutput() {
	//fprintf(stderr, "Initialized ConsoleOutput\n");
}

LoggerConsoleOutput::~LoggerConsoleOutput() {
	//fprintf(stderr, "Destroyed ConsoleOutput\n");
}

// Write the the log entry to console
bool LoggerConsoleOutput::write_log_entry(const std::string &log_entry, int log_level) {
	if (log_level == Logger::ERR) {
		std::cerr << log_entry;
		std::cerr.flush();
	} else {
		std::cout << log_entry;
		std::cout.flush();
	}

	return true;
}

// Default file output implementation
LoggerFileOutput::LoggerFileOutput(const char *output_path) {
	//fprintf(stderr, "Initialized FileOutput with output_path = %s\n", output_path);

	_output_path = output_path;

	// Open the file for appending at the end of the log file
	_fs.open(output_path, std::fstream::out | std::fstream::app);
}

LoggerFileOutput::~LoggerFileOutput() {
	//fprintf(stderr, "Destroyed FileOutput\n");
	// Close our file
	_fs.close();
}

// Write the the log entry to our output
bool LoggerFileOutput::write_log_entry(const std::string &log_entry, int log_level) {
	std::lock_guard<std::mutex> guard(_mutex);

	_fs << log_entry;
	_fs.flush();

	return true;
}
