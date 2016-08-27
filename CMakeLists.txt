cmake_minimum_required (VERSION 3.1)

project (crookie CXX)
set (crookie_VERSION_MAJOR 0)
set (crookie_VERSION_MINOR 2)


#configure_file(
#	"${PROJECT_SOURCE_DIR}/config.h.in"
#	"${PROJECT_BINARY_DIR}/include/config.h"
#)

include_directories("${PROJECT_BINARY_DIR}/include")

set( HEADERS
  MultiThreading/LockedPtr.h
  MultiThreading/SimpleLock.h
  EventBus/ActiveObject.h
  EventBus/AEventHandler.h
  EventBus/EventBase.h
  EventBus/HandlerFunctor.h
  EventBus/Core/EventBus.h
  EventBus/Core/BucketException.h
  EventBus/Core/IEvent.h
  EventBus/Core/IEventHandler.h
  EventBus/Core/MonitoredCollection.h
  EventBus/Static/ActiveObject.h
  EventBus/Static/AEventHandler.h
  EventBus/Static/StaticBus.h
)

add_library(crookie
  EventBus/Core/IEventHandler.cpp
	EventBus/Core/EventBus.cpp

  ${HEADERS}
)

set_property(TARGET crookie PROPERTY CXX_STANDARD 11)
set_property(TARGET crookie PROPERTY CXX_STANDART_REQUIRED ON)


# test

option (BUILD_TESTS "build test modules (use boost test framework)" ON)

if (BUILD_TESTS)
  add_custom_target(check 
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
      COMMAND ${CMAKE_CTEST_COMMAND} --verbose -C ${CMAKE_BUILD_TYPE}
	)
  add_subdirectory(test)
endif (BUILD_TESTS)
