#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../logger.h"

class LoggerStringOutput : public LoggerOutput {
public:
	LoggerStringOutput(std::ostringstream &dest) : _dest(dest) {}
	~LoggerStringOutput() = default;

	bool write_log_entry(const std::string &log_entry, int log_level) override {
		_dest << log_entry;
		return true;
	};

private:

	std::ostringstream &_dest;
};

TEST_CASE("Logger", "Test the logger interface") {
	Logger logger;

	SECTION("Initialization") {
		REQUIRE( logger.get_log_filter() == Logger::INFO );
		REQUIRE( logger.get_log_level() == Logger::INFO );
	}

	SECTION("String output") {
		std::ostringstream dest;
		logger.add_output(move(make_unique<LoggerStringOutput>(dest)));
		logger.set_log_filter(Logger::INFO);

		logger(Logger::INFO) << "Testing\n";

		REQUIRE_THAT( dest.str(), Catch::EndsWith("Testing\n", Catch::CaseSensitive::Yes) );
	}

	SECTION("Filtering") {
		std::ostringstream dest;
		logger.add_output(move(make_unique<LoggerStringOutput>(dest)));
		logger.set_log_filter(Logger::ERR | Logger::WARN);

		logger(Logger::INFO) << "Testing\n";
		REQUIRE_THAT( dest.str(), Catch::Equals("", Catch::CaseSensitive::Yes) );

		logger(Logger::ERR) << "Error msg!\n";
		REQUIRE_THAT( dest.str(), Catch::EndsWith("Error msg!\n", Catch::CaseSensitive::Yes) );
	}

	/*
	SECTION("Console output") {
		logger.add_output(move(make_unique<LoggerConsoleOutput>()));
		logger.add_output(move(make_unique<LoggerConsoleOutput>()));
	}
	*/

	SECTION("File output") {
		logger.add_output(move(make_unique<LoggerFileOutput>("/tmp/log_test.txt")));
	}
}
