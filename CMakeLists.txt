cmake_minimum_required (VERSION 3.1)

project (crookie CXX)
set (crookie_VERSION_MAJOR 0)
set (crookie_VERSION_MINOR 2)


#configure_file(
#	"${PROJECT_SOURCE_DIR}/config.h.in"
#	"${PROJECT_BINARY_DIR}/include/config.h"
#)

include_directories("${PROJECT_BINARY_DIR}/include")

add_library(crookie 
	EventBus/EventBus.cpp
)

set_property(TARGET crookie PROPERTY CXX_STANDARD 11)
set_property(TARGET crookie PROPERTY CXX_STANDART_REQUIRED ON)


# test

option (BUILD_TESTS "build test modules (use boost test framework)" ON)

if (BUILD_TESTS)
  add_custom_target(check 
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
			COMMAND ${CMAKE_CTEST_COMMAND} --verbose
	)
  add_subdirectory(test)
endif (BUILD_TESTS)
