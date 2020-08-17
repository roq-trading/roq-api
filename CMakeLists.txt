cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# version (using git tag)

include(GetGitRepoVersion)

# project

project(roq-api VERSION ${GIT_REPO_VERSION})

# conda

if(DEFINED ENV{CONDA_PREFIX} AND NOT DEFINED ENV{CONDA_BUILD})
  message(WARNING "Setting CMAKE_INSTALL_PREFIX and CMAKE_FIND_ROOT_PATH")
  set(CMAKE_INSTALL_PREFIX $ENV{CONDA_PREFIX})
  set(CMAKE_FIND_ROOT_PATH $ENV{CONDA_PREFIX})
endif()

# c++

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# filesystem

include(GNUInstallDirs)

# dependencies

find_package(Flatbuffers REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(fmt REQUIRED)

find_program(FLATBUFFERS_COMPILER flatc REQUIRED)

# includes

include_directories("${CMAKE_SOURCE_DIR}/include" "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}"
)# auto-generated

# common

set(SCHEMA_DIR "${CMAKE_SOURCE_DIR}/schema")

set(AUTOGEN_PY "${CMAKE_SOURCE_DIR}/scripts/autogen/main.py")

# sub-projects

add_subdirectory("${CMAKE_SOURCE_DIR}/schema/cpp")
add_subdirectory("${CMAKE_SOURCE_DIR}/schema/fbs")

add_subdirectory("${CMAKE_SOURCE_DIR}/include/roq")
add_subdirectory("${CMAKE_SOURCE_DIR}/include/roq/fbs")

# project

add_library(${PROJECT_NAME} INTERFACE)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-schema-cpp)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-schema-fbs)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-include-cpp)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-include-fbs)

# install (public headers)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/include/roq/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/roq"
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "CMakeFiles" EXCLUDE)

# python

option(BUILD_PYTHON "Enable python" OFF)

if(BUILD_PYTHON)
  add_subdirectory("${CMAKE_SOURCE_DIR}/python")
  add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-python)
endif(BUILD_PYTHON)

# test

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if(BUILD_TESTING)
  include(GoogleTest)
  add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/googletest" EXCLUDE_FROM_ALL)
  add_subdirectory("${CMAKE_SOURCE_DIR}/test")
endif(BUILD_TESTING)

# doxygen

option(BUILD_DOCS "Enable doxygen" OFF)

if(BUILD_DOCS)
  find_package(Doxygen)
  add_subdirectory("${CMAKE_SOURCE_DIR}/doxygen")
  add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-doxygen)
  add_dependencies(${PROJECT_NAME}-doxygen ${PROJECT_NAME}-include-cpp)
endif(BUILD_DOCS)

# install (cmake)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config)

set(CMAKE_LIB_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(
  TARGETS ${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAME}::
  FILE "${CMAKE_LIB_DIR}/${PROJECT_NAME}-config.cmake")

install(
  EXPORT ${PROJECT_NAME}-config
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${CMAKE_LIB_DIR}")
