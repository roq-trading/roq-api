set(TARGET_NAME "${PROJECT_NAME}-include-fbs")

set(TARGET_DIR "${CMAKE_INSTALL_INCLUDEDIR}/roq/fbs")

# auto-generate

set(API_FBS "${SCHEMA_DIR}/fbs/api.fbs")
set(API_GENERATED_H "${CMAKE_BINARY_DIR}/${TARGET_DIR}/api_generated.h")
set(API_HPP "${CMAKE_BINARY_DIR}/${TARGET_DIR}/api.hpp")

# check flatbuffers version

execute_process(COMMAND bash "-c" "${FLATBUFFERS_COMPILER} --version | awk '{ print $3 }'"
                OUTPUT_VARIABLE FLATC_VERSION)

message("Using FLATC_VERSION=${FLATC_VERSION}")

# generate the api (depends on flatc version)

if(${FLATC_VERSION} VERSION_LESS "1.11.0")
  message(ERROR "flatc must be version 1.11.0 or newer")
else()
  add_custom_command(
    OUTPUT "${API_HPP}"
    COMMAND "${FLATBUFFERS_COMPILER}" ARGS --binary ARGS --gen-all ARGS --cpp ARGS --cpp-std=c++17
            ARGS "${API_FBS}"
    COMMAND ${CLANG_FORMAT} "${API_GENERATED_H}" > "${API_HPP}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${TARGET_DIR}"
    VERBATIM
    DEPENDS "${FLATBUFFERS_COMPILER}" "${API_FBS}")
endif()

# target

add_custom_target(${TARGET_NAME} ALL DEPENDS "${API_HPP}" "${GITIGNORE}")

# install

install(FILES "${API_HPP}" encode.hpp DESTINATION "${TARGET_DIR}")
