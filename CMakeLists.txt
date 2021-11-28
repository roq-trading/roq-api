cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# version (using git tag)

include(GetGitRepoVersion)

message("Using GIT_REPO_VERSION=${GIT_REPO_VERSION}")

# project

project(roq-api VERSION ${GIT_REPO_VERSION})

# conda

if(DEFINED ENV{CONDA_PREFIX} AND NOT DEFINED ENV{CONDA_BUILD})
  message("Setting CMAKE_INSTALL_PREFIX and CMAKE_FIND_ROOT_PATH")
  set(CMAKE_INSTALL_PREFIX $ENV{CONDA_PREFIX})
  set(CMAKE_FIND_ROOT_PATH $ENV{CONDA_PREFIX})
endif()

# prefer static libraries

set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

# c++

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  message("Setting default visibility to hidden")
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif (CMAKE_BUILD_TYPE STREQUAL "Release")

# filesystem

include(GNUInstallDirs)

# dependencies

find_package(Flatbuffers REQUIRED)
find_package(PythonInterp REQUIRED)

find_package(date REQUIRED)
find_package(fmt REQUIRED)
find_package(magic_enum REQUIRED)
find_package(span-lite REQUIRED)

find_program(CLANG_FORMAT clang-format REQUIRED)
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

install(FILES "${CMAKE_SOURCE_DIR}/CHANGELOG.md"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/${PROJECT_NAME}")

set(CMAKE_LIB_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(
  TARGETS ${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAME}::
  FILE "${CMAKE_LIB_DIR}/${PROJECT_NAME}-config.cmake")

install(
  EXPORT ${PROJECT_NAME}-config
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${CMAKE_LIB_DIR}")

# debian

option(BUILD_DEB "Enable debian packaging?" OFF)

if(BUILD_DEB)
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "thraneh@gmail.com")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libfmt-dev, librange-v3-dev, roq-oss-span-lite")
  include(CPack)
endif(BUILD_DEB)
