set(TARGET_NAME "${PROJECT_NAME}-schema-cpp")

set(SOURCES
  batch_begin.json
  batch_end.json
  cancel_order.json
  connection.json
  connection_status.json
  create_order.json
  download_begin.json
  download_end.json
  error.json
  execution_instruction.json
  fill.json
  funds_update.json
  gateway_status.json
  layer.json
  market_by_order_update.json
  market_by_price_update.json
  market_data_status.json
  market_status.json
  mbo_update.json
  mbp_update.json
  message_info.json
  modify_order.json
  option_type.json
  order_ack.json
  order_manager_status.json
  order_status.json
  order_type.json
  order_update.json
  order_update_action.json
  origin.json
  position_effect.json
  position_update.json
  reference_data.json
  request_status.json
  request_type.json
  security_type.json
  side.json
  start.json
  statistics.json
  statistics_type.json
  statistics_update.json
  stop.json
  time_in_force.json
  timer.json
  top_of_book.json
  trade.json
  trade_summary.json
  trade_update.json
  trading_status.json
)

set(HEADER_DIR "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/roq")

set(AUTOGEN_PY "${CMAKE_SOURCE_DIR}/scripts/autogen/main.py")

unset(AUTOGEN_HEADERS)
foreach(source ${SOURCES})
  get_filename_component(name ${source} NAME_WE)
  set(target "${HEADER_DIR}/${name}.h")
  add_custom_command(
    OUTPUT "${target}"
    COMMAND ${PYTHON_EXECUTABLE} "${AUTOGEN_PY}" --namespace roq --file-type h "${source}" > "${target}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
    DEPENDS ${PYTHON_EXECUTABLE} "${source}")
  list(APPEND AUTOGEN_HEADERS "${target}")
endforeach()

add_custom_target("${TARGET_NAME}" ALL
  DEPENDS ${AUTOGEN_HEADERS})

install(FILES ${AUTOGEN_HEADERS}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/roq")
