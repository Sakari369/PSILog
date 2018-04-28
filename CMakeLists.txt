cmake_minimum_required(VERSION 3.1)

project(PSILogger)
set (CMAKE_CXX_STANDARD 14)

set(SOURCES
        src/main.cpp
	src/logger.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Testing
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/tests)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make test executable
set(TEST_SOURCES
	src/tests/test_logger.cpp
	src/logger.cpp
)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests Catch)
