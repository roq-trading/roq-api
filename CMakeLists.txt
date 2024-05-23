cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# config

include(RoqConfig)

# version (using git tag)

include(GetGitRepoVersion)

message("Using GIT_REPO_VERSION=${GIT_REPO_VERSION}")

# project

project(roq-api VERSION ${GIT_REPO_VERSION})

# filesystem

include(GNUInstallDirs)

# dependencies

find_package(Python3 REQUIRED COMPONENTS Interpreter)

find_package(fmt REQUIRED)
find_package(magic_enum REQUIRED)

find_program(CLANG_FORMAT clang-format REQUIRED)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if(BUILD_TESTING)
  find_package(Catch2 REQUIRED)
endif()

# includes

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR})

# common

set(SCHEMA_DIR ${CMAKE_SOURCE_DIR}/schema)

set(AUTOGEN_PY ${CMAKE_SOURCE_DIR}/scripts/autogen.py)

# sub-projects

add_subdirectory(${CMAKE_SOURCE_DIR}/schema)
add_subdirectory(${CMAKE_SOURCE_DIR}/include/roq)

# project

add_library(${PROJECT_NAME} INTERFACE)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-schema-cpp)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-include-cpp)

# sub-projects

add_subdirectory(${CMAKE_SOURCE_DIR}/src/roq)

# install (public headers)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/roq/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/roq
  FILES_MATCHING
  PATTERN "*.h*"
  PATTERN "CMakeFiles" EXCLUDE)

# doxygen

option(BUILD_DOCS "Enable doxygen" OFF)

if(BUILD_DOCS)
  find_package(Doxygen)
  add_subdirectory(${CMAKE_SOURCE_DIR}/doxygen)
  add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-doxygen)
  add_dependencies(${PROJECT_NAME}-doxygen ${PROJECT_NAME}-include-cpp)
endif()

# install (cmake)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config)

install(FILES ${CMAKE_SOURCE_DIR}/CHANGELOG.md
        DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/${PROJECT_NAME})

set(CMAKE_LIB_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

export(
  TARGETS ${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAME}::
  FILE ${CMAKE_LIB_DIR}/${PROJECT_NAME}-config.cmake)

install(
  EXPORT ${PROJECT_NAME}-config
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_LIB_DIR})
