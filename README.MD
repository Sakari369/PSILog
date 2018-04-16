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

## Some notes from the author

Some design principles and notes behind this programming assignment :) 
This is a pretty simple project, but these things apply on both big and small scale. 

The design was started with the question: "How would the user use this API ?"

This way the implementation is based on the usage. 
This is also a good way to get a clear view immediately what the code requires, what kind of parameters are needed etc.

First I thought about making the logger more functional, but decided that since the requirement was C++ only, it was okay to use the stringstream << approach, which works nicely.
And if you need to interface this from C/Lua or whatever, proxy methods can be built later.

I feel it's important to create simple and predictable code. 
Readability is also very important, the ability to parse the code fast when reading it.

Modularity is also a big thing, how to design code, that can be plugged into existing code in a way that is not hardwired. 
And at the same time keeping in mind, that it's better to first implement with a simple implementation, and then re-factor to be more modular when need raises.

Here are some of the rationale behind my coding conventions:

* Always aim for human readable code
* If it looks too complicated, it probably is
* Easy to read and understand function and variable names.
* Logical and consistent naming conventions. eg. set_surface_color(surface_color)
* I use snake_case for variable names and methods, easier to read vs camelCaseVariables
* Member private variables start with _, eg. _member
* Comment the code, and update comments when functionality changes. 

On every project I work on, I keep a journal, where I solve the problem first on theory level only, if it's anything non-trivial.
This also helps me to get back to any details I've solved once, and keep my head and local memory clean.

This project was implemented using modern C++ practices, using mostly QT Creator with VIM -keybindings, and also a highly customized MacVIM environment.
The cmake was added so that QT Creator can manage the project, and also because it's the easiest way to generate multiplatform makefiles.

For testing, I used the excellent Catch2 library, letting me to focus on the core concepts.
This was a fun assignment to implement, I wrote this partly to replace my simpler logging code in my current C++ projects.

## Future features and ideas how to improve the current version

TODO:
 * Define a log macro, so we can add line number and function name this logger was called from in the prefix
 * Optimize the macro out if NDEBUG or such defined, so that the compiler optimizes the calls out completely if we don't want any
   logging
 * Support customizing the log prefix easily
 * Write tests for multithreading safety, didn't have time to get them working properly, but according to implementation in main.cpp
   usage is thread safe.
 * Handle actual buffer file writing in another thread, resulting in non-locking log calls
