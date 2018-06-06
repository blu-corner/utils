#
# Copyright 2014-2018 Neueda Ltd.
#
include_directories(
  ${CMAKE_INSTALL_PREFIX}/include/
  ${CMAKE_INSTALL_PREFIX}/include/sbf
  ${CMAKE_INSTALL_PREFIX}/include/event2
)
link_directories(
    ${CMAKE_INSTALL_PREFIX}/lib
)

set (INSTALL_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/common.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/compat.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/dl.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/utils.h"
  )

set (SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/dl.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp"
  )

add_library (utils SHARED ${SOURCES})
target_link_libraries (utils sbfcommon dl)

install (TARGETS utils
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install (FILES ${INSTALL_HEADERS} DESTINATION include/utils)

install(
  EXPORT ${PROJECT_NAME}
  DESTINATION lib/cmake/${PROJECT_NAME} COMPONENT dev
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )

set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(${PROJECT_NAME}_LIBRARIES utils)
configure_file(
  ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config-install.cmake
  @ONLY
  )
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config-install.cmake
  DESTINATION lib/cmake/${PROJECT_NAME} COMPONENT dev
  RENAME ${PROJECT_NAME}Config.cmake
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )

