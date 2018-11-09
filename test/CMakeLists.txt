#
# Copyright 2014-2018 Neueda Ltd.
#
set(TESTS_LIBRARIES
  utils
  gtest
  gmock
  )

if(NOT WIN32)
	set(TESTS_LIBRARIES ${TESTS_LIBRARIES}
						pthread)
endif()

include_directories(
  ${CMAKE_BINARY_DIR}/googletest/include
  ${PROJECT_SOURCE_DIR}/test
  ${PROJECT_SOURCE_DIR}/src
  )
link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${CMAKE_BINARY_DIR}/googletest/lib)

add_executable(unittest
  testRunner.cpp
  testUtils.cpp
  testDl.cpp
  )

add_library(testinput SHARED
  testInput.cpp
  )

target_link_libraries(unittest ${TESTS_LIBRARIES})
add_dependencies(unittest googletest)

add_test(NAME unittest
  COMMAND unittest --gtest_output=xml:../test.xml
)

install(TARGETS unittest testinput
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib)
