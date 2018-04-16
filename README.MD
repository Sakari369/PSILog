# Rightware Logger

Rightware Logger is a C++11 logger class that let's you easily and flexibly add logging support to your project.
The design principles for this library have been simplicity of use and flexibility to suit any use case.

Builtin with support for logging into console or files, it supports chaining log messages into multiple outputs.
Extending outputters is also easy, the user can write their own log message output destination and add to the chain.

## Features

* Implemented with cross-platform C++11
* Log easily to multiple target destinations
* Easy and simple to use
* Multithreading safety
* Extendable, write your own log destinations to suit any use case

## Requirements

* C++11 compatible compiler (tested with clang/LLVM 4)
* CMake 3.1 or newer
* Possibly macOS, written and tested only on macOS High Sierra, should be crossplatform though

## Usage

Include the `logger.h` in your code, and add `logger.cpp` into your project compilation.

### Code Example

```cpp
	Logger logger;

	logger.set_log_filter(Logger::INFO | Logger::WARN | Logger::ERR);

	logger.add_output(move(make_unique<LoggerConsoleOutput>()));
	logger.add_output(move(make_unique<LoggerFileOutput>("/tmp/log_test.txt")));

	logger(Logger::INFO) << "All systems initialized" << std::endl;
	logger(Logger::WARN) << "WARNING: Phasers damaged" << std::endl;
	logger.flush(); // force flushing of output

	logger(Logger::ERR)  << "ERROR: Failed to boot phasers" << std::endl;
```

## Compiling

(From the root directory)

```
mkdir build
cd build
cmake ..
make
```

This will generate the Makefile in the build directory.
From there you can run make to compile the project.

## Running

Run `./RightwareLogger` to run a test implementation
Run `./run_tests` to run the tests

## Design principles and personal preferences in code style

Some design principles and notes behind this programming assignment :) This is a pretty simple project, but these things apply on both big and small scale. The design was started from the view of how would the user use this API ?

This way the implementation is based on the usage. This is also a good way to get a clear view immediately what the code requires to be implemented, what kind of parameters and other stuff you need to pass to it and so on, and you get to use it right away, iterating on the usage immediately.

I feel it's important to create simple and predictable code, that you can figure out logically how it works, without having to look at the documentation too much. Readability is also very important, that you can parse the code fast when reading it, making sense of what is happening when we call this function for example.

Modularity is also a big thing, how to design code that can be plugged into existing code in a way that is not hardwired, and how to write code that can be extendable and used in many situations. And at the same time keeping in mind, that it's better to first implement with a simple implementation, and then re-factor to be more modular when need raises, getting to working solutions fast without spending too much time up front figuring out a over-generalized or optimized solution.

Here is some of the rationale behind my coding conventions and ways I have found to be effective:

* Always aim for human readable code, eg. keep one operation per line of code if possible
* Easy to read and understand function and variable names.
* Logical and consistent naming conventions. eg. set_surface_color(surface_color)
* I use snake_case for variable names and methods, it has been studied that the brain will automatically replace _ with a space
* Same thing for member variables, _member is available with modern compilers
* Comment the code, and update comments when functionality changes. 
* I always keep a journal, where I solve the problem first on paper, then move into implementation, if the problem is anything but trivial

The target environment to be used was C++11, leveraging modern features like unique pointers and using stream overriding to handle the logging, as to support the native way C++ developers have used to, and at the same time easily supporting logging user defined types, by just overriding the << operator in the user defined type, again conforming to modern C++ practices.

## Future features and ideas how to improve the current version

TODO:
 * Define a log macro, so we can add line number and function name this logger was called from in the prefix
 * Optimize the macro out if NDEBUG or such defined, so that the compiler optimizes the calls out completely if we don't want any
   logging
 * Support customizing the log prefix easily
 * Write tests for multithreading safety, didn't have time to get them working properly, but according to implementation in main.cpp
   usage is thread safe.
