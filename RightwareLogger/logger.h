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

        template<class T>
	Logger& operator << (const T& output) {
		// Append log string into our log stream
		if (_log_level & _log_filter) {
			// If the log output hasn't been flushed to the destination yet, append
			// log entry prefix to the stream
			if (_new_entry == true) {
				std::string prefix = get_log_entry_prefix(_log_stream.str());
				_log_stream << prefix;
				_new_entry = false;
			}

			_log_stream << output;
		}

                return *this;
        }

	// Manipulation functions, endl, flush, setw etc
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
				_new_entry = true;
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

	// For setting the log level while calling as a functor
	Logger& operator () (LogLevel log_level) {
                _log_level = log_level;
                return *this;
        }

	// Flush the output to our output class
	void flush();

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

	// Used to detect if we should append the prefix to the entry or not
	bool _new_entry = true;

	// Our log message outputters chain
	// We dispatch the actual log messages to these in sequential order
	// TODO: We should also have a default outputter if none is assigned that outputs to the console
	std::vector<unique_ptr<LoggerOutput>> _outputters;

	// The stream where we buffer our log messages until flushing
	std::stringstream _log_stream;
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
