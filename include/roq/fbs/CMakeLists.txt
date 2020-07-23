set(TARGET_NAME "${PROJECT_NAME}-include-fbs")

set(TARGET_DIR "${CMAKE_INSTALL_INCLUDEDIR}/roq/fbs")

# auto-generate

set(API_FBS "${SCHEMA_DIR}/fbs/api.fbs")
set(API_H "${CMAKE_BINARY_DIR}/${TARGET_DIR}/api.h")

add_custom_command(
  OUTPUT "${API_H}"
  COMMAND "${FLATBUFFERS_COMPILER}"
  ARGS --binary
  ARGS --gen-all
  ARGS --filename-suffix ""
  ARGS --cpp --cpp-std c++17
  ARGS "${API_FBS}"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${TARGET_DIR}"
  VERBATIM
  DEPENDS "${FLATBUFFERS_COMPILER}" "${API_FBS}")

# gitignore

set(GITIGNORE "${CMAKE_CURRENT_SOURCE_DIR}/.gitignore")

file(WRITE "${GITIGNORE}" "api.h\n")

# target

add_custom_target(${TARGET_NAME} ALL
  DEPENDS "${API_H}" "${GITIGNORE}")

# install

install(FILES "${API_H}"
  DESTINATION "${TARGET_DIR}")
