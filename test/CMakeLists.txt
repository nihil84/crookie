cmake_minimum_required (VERSION 3.1)
enable_testing ()

project (UnitTests)

file (GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/ *.cpp)

foreach (testSrc ${TEST_SRCS})

  # extract filename without extension
  get_filename_component (testName ${testSrc} NAME_WE)

  message ("adding test ${testName}")

  add_executable (${testName} ${testSrc})
  
  target_link_libraries (${testName} crookie)
  
  set_target_properties (${testName} PROPERTIES 
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
  )
  
  add_test(NAME ${testName} 
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName}
  )

endforeach (testSrc)
