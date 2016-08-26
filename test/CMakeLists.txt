cmake_minimum_required (VERSION 3.1)
enable_testing ()

project (UnitTests)

find_package (Threads)

file (GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/ *.cpp)

foreach (testSrc ${TEST_SRCS})

  # extract filename without extension
  get_filename_component (testName ${testSrc} NAME_WE)

  message ("adding test ${testName}")

  add_executable (${testName} ${testSrc})
  
  include_directories ("${crookie_SOURCE_DIR}")
  target_link_libraries (${testName} crookie)
  target_link_libraries (${testName} ${CMAKE_THREAD_LIBS_INIT})
  
  set_target_properties (${testName} PROPERTIES 
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
  )

  set_property(TARGET ${testName} PROPERTY CXX_STANDARD 11)
  set_property(TARGET ${testName} PROPERTY CXX_STANDART_REQUIRED ON)
  
  add_test(NAME ${testName} 
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/${testName}
  )

  add_dependencies(check ${testName})

endforeach (testSrc)
