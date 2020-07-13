set(TARGET_NAME "${PROJECT_NAME}-autogen-headers")

file(GLOB SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_LIST_DIR}/*.json")

set(HEADER_DIR "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/roq")

set(AUTOGEN_PY "${CMAKE_SOURCE_DIR}/scripts/autogen/main.py")

foreach(source ${SOURCES})
  get_filename_component(name ${source} NAME_WE)
  set(target "${HEADER_DIR}/${name}.h")
  add_custom_command(
    OUTPUT "${target}"
    COMMAND ${PYTHON_EXECUTABLE} "${AUTOGEN_PY}" --namespace roq --file-type h "${source}" > "${target}"
    VERBATIM
    DEPENDS ${PYTHON_EXECUTABLE} "${source}")
  list(APPEND AUTOGEN_HEADERS "${target}")
endforeach()

add_custom_target("${TARGET_NAME}" ALL
  DEPENDS ${AUTOGEN_HEADERS})

install(FILES ${AUTOGEN_HEADERS}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/roq")
