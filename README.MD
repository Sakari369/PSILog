# Rightware Logger

Rightware Logger is a single file implementation of a C++11 logger that let's you easily and flexibly add logging support to your project.
The design principles for this library have been simplicity of use and flexibility to suit any use case.

## Features

* Implemented with cross-platform C++11
* Log easily to multiple target destinations
* Easy and simple to use
* Multithreading safety
* Extendable, write your own log destinations to suit any use case

## Requirements

* C++11 compatible compiler (tested with clang/LLVM 4)
* CMake 3.1 or newer

## Usage

Include the `logger.h` in your code, and add `logger.cpp` into your project compilation.

### Examples

```cpp
	Logger logger;

	logger.set_log_filter(Logger::INFO | Logger::WARN | Logger::ERR);

	logger.add_output(move(make_unique<LoggerConsoleOutput>()));
	logger.add_output(move(make_unique<LoggerFileOutput>("/tmp/log_test.txt")));

	logger(Logger::INFO) << "All systems initialized" << std::endl;
	logger(Logger::WARN) << "WARNING: Phasers damaged" << std::endl;
	logger(Logger::ERR)  << "ERROR: Failed to boot phasers" << std::endl;
```

## Compiling

Run `cmake -f CMakeLists.txt` and `make` to build the library.
Use `make tests` to run the tests.
