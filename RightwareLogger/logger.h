// Logger.h
// A logger class

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

// Implementation for C++11
#include "make_unique.h"

class LoggerOutput;
class LoggerConsoleOutput;

class Logger {

public:
        enum LogLevel {
                NONE	= 0,
                INFO	= 1,
                WARN	= 2,
                ERR	= 2 << 1,
                FREQ	= 2 << 2
        };

        Logger();
        ~Logger();

        template<class T>
	Logger& operator << (const T& output) {
		if (_log_level & _log_filter) {
			_output_stream << output;
		}

                return *this;
        }

	// Manipulation functions, endl, flush, setw etc
	typedef std::ostream& (*ManipFn)(std::ostream &);
	Logger& operator << (ManipFn manip) {
		if (_log_level & _log_filter) {
			// Apply the operation
			manip(_output_stream);

			// Check if we should flush the output
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
			manip(_output_stream);
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

	// Add new logger to our output chain
	// Basically we would like to own this as an unique_ptr
	bool add_output(std::unique_ptr<LoggerOutput> output);

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

	// Our log message outputters chain
	// We dispatch the actual log messages to these in sequential order
	std::unique_ptr<LoggerOutput> _outputter;

	// The stream where we buffer our log messages until flushing
        std::stringstream _output_stream;
};

// Super class for implementing logger outputs
//
class LoggerOutput {
public:
	LoggerOutput() = default;
	~LoggerOutput() = default;

	// TODO: maybe use a struct for the log entry
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
