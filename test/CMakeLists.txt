#
# Copyright 2014-2018 Neueda Ltd.
#
include_directories(
  ${CMAKE_INSTALL_PREFIX}/include/
  ${CMAKE_INSTALL_PREFIX}/include/sbf
  ${CMAKE_INSTALL_PREFIX}/include/event2
  ${CMAKE_BINARY_DIR}/googletest/include
  ${CMAKE_INSTALL_PREFIX}/include/config
  ${PROJECT_SOURCE_DIR}/test
  ${PROJECT_SOURCE_DIR}/src
  )
link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${CMAKE_BINARY_DIR}/googletest/lib)

add_executable(unittest
  testRunner.cc
  testUtils.cc
  testDl.cc
  )

add_library(testinput SHARED
  testInput.cc
  )

target_link_libraries(unittest
  utils
  gtest
  gmock
  pthread
  )

add_test(NAME unittest
  COMMAND unittest --gtest_output=xml:../test.xml
)

install(TARGETS unittest testinput
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib)
