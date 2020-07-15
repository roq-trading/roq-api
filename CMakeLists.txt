cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# version (using git tag)

include(GetGitRepoVersion)

project(roq-api VERSION ${GIT_REPO_VERSION})

# conda

if(DEFINED ENV{CONDA_PREFIX})
  set(CMAKE_INSTALL_PREFIX $ENV{CONDA_PREFIX})
  set(CMAKE_FIND_ROOT_PATH $ENV{CONDA_PREFIX})
endif()

# c++

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# filesystem

include(GNUInstallDirs)

# dependencies

find_package(Flatbuffers REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(fmt REQUIRED)

# include

include_directories(
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")  # autogen

# ensure required output directories exist

add_custom_target(ensure-include-directory-exists ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_INSTALL_INCLUDEDIR}/roq")

# configure

set(ROQ_API_H "${CMAKE_INSTALL_INCLUDEDIR}/roq/api.h")

configure_file("${CMAKE_SOURCE_DIR}/include/roq/api.h.in"
  "${ROQ_API_H}"
  @ONLY)

install(FILES "${CMAKE_BINARY_DIR}/${ROQ_API_H}"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/roq")

# headers (public)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/roq/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/roq"
  FILES_MATCHING PATTERN "*.h")

# sub-projects

add_subdirectory("${CMAKE_SOURCE_DIR}/schema/cpp")
add_subdirectory("${CMAKE_SOURCE_DIR}/schema/flatbuffers")

# project

add_library(${PROJECT_NAME} INTERFACE)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-autogen-headers)

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
  add_dependencies(${PROJECT_NAME}-doxygen ${PROJECT_NAME}-autogen-headers)
endif(BUILD_DOCS)

# install

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-config
)

set(ROQ_API_CMAKE_DIR
  "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(TARGETS ${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAME}::
  FILE "${ROQ_API_CMAKE_DIR}/${PROJECT_NAME}-config.cmake"
)

install(EXPORT ${PROJECT_NAME}-config
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${ROQ_API_CMAKE_DIR}"
)
