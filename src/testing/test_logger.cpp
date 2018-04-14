// test_logger.cpp
//
// RightWare logger tests, using Catch2 test framework
//
// Copyright (c) 2018 Sakari Lehtonen <sakari AT psitriangle DOT net>

#define CATCH_CONFIG_MAIN

#include <fstream>
#include <cstdio>

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

private:
	std::ostringstream &_dest;
};

TEST_CASE("Logger", "Test the logger interface") {
	Logger logger;
	std::string log_path = "/tmp/logger_test.txt";

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

		logger(Logger::INFO) << "Testing\n";

		REQUIRE_THAT( dest.str(), Catch::EndsWith("Testing\n", Catch::CaseSensitive::Yes) );
	}

	SECTION("Filtering") {
		std::ostringstream dest;
		logger.add_output(move(make_unique<LoggerStringOutput>(dest)));

		// Set filter level so that the next log message wont go through
		logger.set_log_filter(Logger::ERR | Logger::WARN);

		// Test if the filter works, the string should be empty
		logger(Logger::INFO) << "Testing\n";
		REQUIRE_THAT( dest.str(), Catch::Equals("", Catch::CaseSensitive::Yes) );

		// ERR should go through
		logger(Logger::ERR) << "Error msg!\n";
		REQUIRE_THAT( dest.str(), Catch::EndsWith("Error msg!\n", Catch::CaseSensitive::Yes) );
	}

	SECTION("File output") {
		// Remove the previous log file if exists
		logger.add_output(move(make_unique<LoggerFileOutput>(log_path.c_str())));

		// The file should be open
		std::ifstream fs(log_path.c_str());
		REQUIRE( fs.good() == true );

		// This shouldn't write anything to the file
		logger(Logger::WARN) << "Warning\n";

		// Get the contents of the file
		std::stringstream buf;
		buf << fs.rdbuf();
		REQUIRE( buf.str().size() == 0);

		// TODO: is this written to the file at this point ?
		// Should we have a flush method that forces the output right away ?
		logger(Logger::INFO) << "Info message\n";
		buf << fs.rdbuf();
		std::cout << buf.str();
		REQUIRE_THAT( buf.str(), Catch::EndsWith("Info message\n", Catch::CaseSensitive::Yes) );

		/*
		std::ifstream f(log_path.c_str());
		REQUIRE( f.good() == false );
		*/

		//
		//REQUIRE_THAT( dest.str(), Catch::Equals("", Catch::CaseSensitive::Yes) );

		//logger(Logger::ERR) << "Error msg!\n";
		//REQUIRE_THAT( dest.str(), Catch::EndsWith("Error msg!\n", Catch::CaseSensitive::Yes) );
	
		fs.close();
	}
}
