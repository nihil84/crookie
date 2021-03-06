cmake_minimum_required (VERSION 3.1)

project (crookie CXX)
set (crookie_VERSION_MAJOR 0)
set (crookie_VERSION_MINOR 2)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)


#configure_file(
#	"${PROJECT_SOURCE_DIR}/config.h.in"
#	"${PROJECT_BINARY_DIR}/include/config.h"
#)

include_directories("${PROJECT_BINARY_DIR}/include")

set( HEADERS
  Utils/ScopeGuard.h
  MultiThreading/LockedPtr.h
  MultiThreading/SimpleLock.h
  EventBus/ActiveObject.h
  EventBus/AEventHandler.h
  EventBus/EventBase.h
  EventBus/HandlerFunctor.h
  EventBus/Core/EventBus.h
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

#find_package (Threads REQUIRED)


# tests

option (BUILD_TESTS "build test modules (use boost test framework)" ON)

if (BUILD_TESTS)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage")
endif ()

  add_custom_target(check 
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
      COMMAND ${CMAKE_CTEST_COMMAND} --verbose -C ${CMAKE_BUILD_TYPE}
	)
	
  add_subdirectory(test)
	
	if (CMAKE_COMPILER_IS_GNUCXX)

		find_program( GCOV_PATH gcov )
		find_program( GCOVR_PATH gcovr)

		add_custom_target(coverage
				${GCOVR_PATH} -x -r ${CMAKE_SOURCE_DIR} -e '${CMAKE_SOURCE_DIR}/test/'  -o coverage.xml
				WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test/reports
				DEPENDS check
				COMMENT "Running gcovr to produce Cobertura code coverage report."
		)
	endif ()

endif (BUILD_TESTS)
