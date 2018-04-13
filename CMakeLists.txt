cmake_minimum_required(VERSION 3.1)

project(RightwareLogger)
set (CMAKE_CXX_STANDARD 11)

set(SOURCES
        src/main.cpp
        src/logger.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/testing)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make test executable
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/testing/test_logger.cpp)
add_executable(test_logger ${TEST_SOURCES})
target_link_libraries(test_logger Catch)
