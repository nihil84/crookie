cmake_minimum_required (VERSION 3.1)
enable_testing ()

project (UnitTests)

find_package (Threads REQUIRED)

set (Boost_USE_STATIC_LIBS ON)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)

if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)

  message(FATAL_ERROR "Unable to find liboost_unit_test_framework library \
which is needed to compile crookie tests. You can provide boost libraries \
install directory with the BOOST_ROOT CMake variable.")

endif (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)


file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/reports)

file (GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/ *.cpp)

foreach (testSrc ${TEST_SRCS})

  # extract filename without extension
  get_filename_component (testName ${testSrc} NAME_WE)

  message ("adding test ${testName}")

  add_executable (${testName} ${testSrc})
  
  include_directories ("${crookie_SOURCE_DIR}" "${Boost_INCLUDE_DIRS}")
  target_link_libraries (${testName} crookie)
  target_link_libraries (${testName} ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries (${testName} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  set_target_properties (${testName} PROPERTIES 
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
  )

  set_property(TARGET ${testName} PROPERTY CXX_STANDARD 11)
  set_property(TARGET ${testName} PROPERTY CXX_STANDART_REQUIRED ON)
  
  add_test(NAME ${testName} 
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/reports
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/${testName} --report_format=XML 
              --report_level=detailed --report_sink=${testName}_report.xml
  )

  add_dependencies(check ${testName})

endforeach (testSrc)
