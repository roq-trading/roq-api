configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  @ONLY)

add_custom_target(${PROJECT_NAME}-doxygen ALL
  COMMAND ${DOXYGEN_EXECUTABLE} "Doxyfile"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

# XXX install?
