// Logger.h
// A logger class

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>

// Implementation for C++11
#include "make_unique.h"

using std::unique_ptr;
using std::make_unique;
using std::move;

class LoggerOutput;
class LoggerConsoleOutput;
class LogStream;

class Logger {

public:
        enum LogLevel {
		NONE	=  0,
		INFO	=  1,
		WARN	=  2,
		ERR	=  2 << 1,
		FREQ	=  2 << 2,
		ALL	= (2 << 3) - 1
        };

        Logger();
        ~Logger();

	// Functors for returning a log stream
	LogStream operator ()();
	LogStream operator ()(int log_level);

	// Public methods

	void log(const std::string &entry, int log_level);

	// Flush the output to our output class
	//void flush();

	std::string get_log_entry_prefix(const std::string &log_entry) const;

	// Add new logger to our output chain
	// Basically we would like to own this as an unique_ptr
	bool add_output(unique_ptr<LoggerOutput> output);

        // Accessors
        int get_log_level() const;
        void set_log_level(int log_level);

        int get_log_filter() const;
        void set_log_filter(int log_filter);

private:
	// The current log level we are logging messages with
        int _log_level = LogLevel::INFO;

	// The log filter that filters the output, compared against the current
	// log level. Binary logic.
        int _log_filter = LogLevel::INFO;

	// Have we flushed the current log stream ?
	bool _flushed = false;

	std::mutex _mutex;

	// Our log message outputters chain
	// We dispatch the actual log messages to these in sequential order
	// TODO: We should also have a default outputter if none is assigned that outputs to the console
	std::vector<unique_ptr<LoggerOutput>> _outputters;

	// The stream where we buffer our log messages until flushing
	//std::stringstream _log_stream;
};

// Stream class for thread safety
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
		_logger.log(str(), _log_level);
	}

	/*
	template<class T>
	Logger& operator << (const T& output) {
		std::stringstream entry;

		if (_log_level & _log_filter) {
			entry << output;
			log(entry.str());
		}

		return *this;
	}
	*/

	// Manipulation functions, endl, flush, setw etc
	/*
	typedef std::ostream& (*ManipFn)(std::ostream &);
	Logger& operator << (ManipFn manip) {
		if (_log_level & _log_filter) {
			// Apply the string operation
			manip(_log_stream);

			// Should we flush the log stream ?
			// This will signal the underlying outputters to flush the output to whatever
			// they are writing it to
			if (manip == static_cast<ManipFn>(std::flush)
			 || manip == static_cast<ManipFn>(std::endl)) {
				this->flush();
			}
		}

		return *this;
	}

	// For setiosflags, resetiosflags
	typedef std::ios_base& (*FlagsFn)(std::ios_base &);
	Logger& operator << (FlagsFn manip) {
		if (_log_level & _log_filter) {
			manip(_log_stream);
		}

		return *this;
	}
	*/

private:
	Logger &_logger;
	int _log_level;
};

// Super class for implementing logger outputs
class LoggerOutput {
public:
	LoggerOutput() = default;
	~LoggerOutput() = default;

	// TODO: maybe use a struct for the log entry

	// This will write the current log entry to the destination output, ensuring that
	// the output is flushed also
	virtual bool write_log_entry(const std::string &log_entry, int log_level) = 0;

protected:
	std::mutex _mutex;

	//virtual void clear_log();
};

// Default implementation of outputting log messages to the console
class LoggerConsoleOutput : public LoggerOutput {
public:
	LoggerConsoleOutput();
	~LoggerConsoleOutput();

	bool write_log_entry(const std::string &log_entry, int log_level) override;
};

// Default implementation of outputting to a file
class LoggerFileOutput : public LoggerOutput {
public:
	LoggerFileOutput(const char *output_path);
	~LoggerFileOutput();

	bool write_log_entry(const std::string &log_entry, int log_level) override;

private:
	const char *_output_path = "";
	std::fstream _fs;
};
