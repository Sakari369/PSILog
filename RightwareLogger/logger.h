// Logger.h
// A logger class

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

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


	// So, if we would like to just ignore the log messages when log filter doesn't match
	// to the current logging level
	// So, basically we could implement this like this:
	// Either define a function that is called in all these overloads that checks
	// if the current logging level matches that of the log filter, and return if not.
	// Or, define some kind of macro that checks the filtering level before even actually calling <<
	// The flush() should only be called if the log level matches the filter

        // For types that implement <<
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

        // Accessors
        int get_log_level() const;
        void set_log_level(int log_level);

        int get_log_filter() const;
        void set_log_filter(int log_filter);

        void flush();

private:
	// The current log level we are logging messages with
        int _log_level = LogLevel::INFO;

	// The log filter that filters the output, compared against the current
	// log level. Binary logic.
        int _log_filter = LogLevel::INFO;

        std::stringstream _output_stream;
};
