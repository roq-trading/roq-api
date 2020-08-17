set(TARGET_NAME "${PROJECT_NAME}-python")

# note! setup.py doesn't do out-of-tree ==> must use SOURCE_DIR
set(MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/roq")

# auto-generate

file(MAKE_DIRECTORY "${MODULE_DIR}/fbs")

set(FBS_API_SOURCE "${CMAKE_SOURCE_DIR}/schema/fbs/api.fbs")

set(FBS_API_TARGET "${MODULE_DIR}/fbs/__init__.py")

add_custom_command(
  OUTPUT "${FBS_API_TARGET}"
  COMMAND "${FLATBUFFERS_COMPILER}" ARGS --binary ARGS --gen-all ARGS --python ARGS
          "${FBS_API_SOURCE}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
  VERBATIM
  DEPENDS "${FBS_API_SOURCE}")

# build python module

set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/lib/roq")

file(MAKE_DIRECTORY "${BUILD_DIR}/fbs")

set(${TARGET_NAME}-build "${BUILD_DIR}/fbs/__init__.py")

add_custom_command(
  OUTPUT ${${TARGET_NAME}-build}
  COMMAND "${PYTHON_EXECUTABLE}" ARGS "setup.py" "build"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  VERBATIM
  DEPENDS "${FBS_API_TARGET}")

# dependency

add_custom_target(${TARGET_NAME} ALL DEPENDS "${${TARGET_NAME}-build}")

# test XXX doesn't work using "make test"...?

add_test(
  NAME ${TARGET_NAME}
  COMMAND "${PYTHON_EXECUTABLE}" "setup.py" "test"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
