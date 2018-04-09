cmake_minimum_required(VERSION 3.1)

project(RightwareLogger)
set (CMAKE_CXX_STANDARD 11)

set(SOURCES
        main.cpp
        logger.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
