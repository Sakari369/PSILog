// test_logger.cpp
//
// RightWare logger tests, using Catch2 test framework
//
// Copyright (c) 2018 Sakari Lehtonen <sakari AT psitriangle DOT net>

#define CATCH_CONFIG_MAIN

#include <fstream>
#include <cstdio>
#include <thread>

#include "catch.hpp"
#include "../logger.h"

// Extending the logger output, so that records to the
// stringstream we provide to this class, so we can test with a stringstream instead of
// having to figure out how to capture console output
class LoggerStringOutput : public LoggerOutput {
public:
	LoggerStringOutput(std::ostringstream &dest) : _dest(dest) {}
	~LoggerStringOutput() = default;

	bool write_log_entry(const std::string &log_entry, int log_level) override {
		_dest << log_entry;
		return true;
	}

	void flush() override {
		_dest.flush();
	}

private:
	std::ostringstream &_dest;
};

TEST_CASE("Logger", "Test the logger interface") {
	Logger logger;
	std::string log_path = "logger_tests.txt";

	// Delete our log file if one exists, so we can start from a clean slate
	std::ifstream fs(log_path.c_str());
	if (fs.good() == true) {
		std::cout << "Log file '" << log_path << "' exists, deleting ..";
		if (std::remove(log_path.c_str()) != 0) {
			FAIL("Failed removing log file, can't test file operations");
		} else {
			std::cout << ". done!" << std::endl;
		}
	}
	fs.close();

	SECTION("Initialization") {
		// Basic levels of logging should be these
		REQUIRE( logger.get_log_filter() == Logger::INFO );
		REQUIRE( logger.get_log_level() == Logger::INFO );
	}

	SECTION("String output") {
		std::ostringstream dest;

		// Test output to a stringstream.
		logger.add_output(move(make_unique<LoggerStringOutput>(dest)));
		logger.set_log_filter(Logger::INFO);

		logger(Logger::INFO) << "String output\n";

		REQUIRE_THAT( dest.str(), Catch::EndsWith("String output\n", Catch::CaseSensitive::Yes) );
	}

	SECTION("Filtering") {
		std::ostringstream dest;
		logger.add_output(move(make_unique<LoggerStringOutput>(dest)));

		// Set filter level so that the next log message wont go through
		logger.set_log_filter(Logger::ERR | Logger::WARN);

		logger(Logger::INFO) << "This is filtered\n";
		REQUIRE_THAT( dest.str(), Catch::Equals("", Catch::CaseSensitive::Yes) );

		logger(Logger::ERR) << "Error msg through the filter\n";
		REQUIRE_THAT( dest.str(), Catch::EndsWith("Error msg through the filter\n", Catch::CaseSensitive::Yes) );
	}

	SECTION("File output") {
		logger.add_output(move(make_unique<LoggerFileOutput>(log_path.c_str())));

		// Check that the LoggerFileOutput created the log file
		std::ifstream in(log_path.c_str());
		REQUIRE( in.good() == true );
		in.close();

		logger(Logger::WARN) << "Warning that is not written" << std::endl;
		logger(Logger::INFO) << "Info message to the file" << std::endl;

		// Read the contents back in
		in.open(log_path.c_str());
		std::string contents((std::istreambuf_iterator<char>(in)), 
				      std::istreambuf_iterator<char>());
		in.close();

		CAPTURE(contents);
		REQUIRE_THAT(contents, Catch::EndsWith("Info message to the file\n", Catch::CaseSensitive::Yes) );
	}
}
