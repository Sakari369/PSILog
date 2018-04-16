// Logger.h
//
// RightWare logger assignment
// Class for handling logging, with support for multiple outputs, thread safety and modular extensions of
// user configurable output destinations
//
// Copyright (c) 2018 Sakari Lehtonen <sakari AT psitriangle DOT net>

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>

// std::make_unique template implementation for C++11
#include "make_unique.h"

using std::unique_ptr;
using std::make_unique;
using std::move;

class LoggerOutput;
class LoggerConsoleOutput;
class LogStream;

// Our main logger class
class Logger {

public:
	// Binary arithmetic current log level mask
        enum LogLevel {
		NONE	=  0,
		INFO	=  1,
		WARN	=  2,
		ERR	=  2 << 1,
		FREQ	=  2 << 2,
		ALL	= (2 << 3) - 1
        };

	Logger() = default;
	~Logger() = default;

	// Functors for returning a log stream, enabling multithreading safe logging
	LogStream operator ()();
	LogStream operator ()(int log_level);

	// The main logging method
	void log(const std::string &entry, int log_level);

	// Return the log message prefix header
	std::string get_log_entry_prefix(const std::string &log_entry) const;

	// Add new logger to our output chain
	// We have multiple output destinations which implement the actual writing of the messages
	// This enables easy extending of log destinations by the user
	void add_output(unique_ptr<LoggerOutput> output);

	// Flush all output now to the destination outputs
	void flush();

	// Pure accessors written here for easier implementation
        int get_log_level() const { return _log_level; }
        void set_log_level(int log_level) { _log_level = log_level; }

	int get_log_filter() const { return _log_filter; }
        void set_log_filter(int log_filter) { _log_filter = log_filter; }

	bool get_add_prefix() const { return _add_prefix; }
	void set_add_prefix(bool add_prefix) { _add_prefix = add_prefix; }

private:
	// The current log level we are logging messages with
        int _log_level = LogLevel::INFO;

	// The log filter that filters the output, compared against the current
	// log level. Binary arithmetic mask.
        int _log_filter = LogLevel::INFO;

	// Do we add the log message prefix to our entries ?
	bool _add_prefix = true;

	// Our log message outputters chain
	// We dispatch the actual log messages to these in sequential order
	std::vector<unique_ptr<LoggerOutput>> _outputters;
};

// Stream class for thread safety
// Temporary instance of this class is returned when
// logger() << "Log entry" << std::endl;
// Or the << operation is called
// Meaning the calling thread gets a temporary LogStream object
// And when in the calling thread the object is destroyed, it actually logs the log messages
// This enables thread safe log message construction, without multiple threads intefering
// with each other
class LogStream : public std::ostringstream {
public:
	// Store reference to the current log level and logger object
	LogStream(Logger &logger, int log_level) :
		_logger(logger), _log_level(log_level)
	{}

	// Copy constructor
	LogStream(const LogStream &ls) :
		_logger(ls._logger),
		_log_level(ls._log_level)
	{}

	~LogStream() {
		// Filter log messages with the binary arithmetic mask
		if (_logger.get_log_filter() & _log_level) {
			_logger.log(str(), _log_level);
		}
	}

private:
	Logger &_logger;
	int _log_level;
};

// The logger outputs to LoggerOutput objects

// Base class for implementing logger outputs
// This allows modular extension of the outputs by the user,
// Just provide an implementatin extending this class, and call add_output()
class LoggerOutput {
public:
	LoggerOutput() = default;
	~LoggerOutput() = default;

	// This will write the current log entry to the destination output, ensuring that
	// the output is flushed also
	virtual bool write_log_entry(const std::string &log_entry, int log_level) = 0;

	// Provide a way to implement flushing the output manually
	virtual void flush() = 0;
};

// Default implementation of outputting log messages to the console
class LoggerConsoleOutput : public LoggerOutput {
public:
	LoggerConsoleOutput() = default;
	~LoggerConsoleOutput() = default;

	bool write_log_entry(const std::string &log_entry, int log_level) override;
	void flush() override;
};

// Default implementation of outputting to a file
class LoggerFileOutput : public LoggerOutput {
public:
	LoggerFileOutput(const char *output_path);
	~LoggerFileOutput();

	bool write_log_entry(const std::string &log_entry, int log_level) override;
	void flush() override;

private:
	const char *_output_path = "";
	std::fstream _fs;
	std::mutex _mutex;
};
