set(TARGET_NAME "${PROJECT_NAME}-schema-fbs")

set(SOURCES
  api.fbs
  cancel_order.fbs
  connection_status.fbs
  create_order.fbs
  download_begin.fbs
  download_end.fbs
  error.fbs
  execution_instruction.fbs
  fill.fbs
  funds_update.fbs
  gateway_status.fbs
  layer.fbs
  market_by_order_update.fbs
  market_by_price_update.fbs
  market_data_status.fbs
  market_status.fbs
  mbo_update.fbs
  mbp_update.fbs
  modify_order.fbs
  option_type.fbs
  order_ack.fbs
  order_manager_status.fbs
  order_status.fbs
  order_type.fbs
  order_update.fbs
  order_update_action.fbs
  origin.fbs
  position_effect.fbs
  position_update.fbs
  reference_data.fbs
  request_status.fbs
  request_type.fbs
  security_type.fbs
  side.fbs
  statistics.fbs
  statistics_type.fbs
  statistics_update.fbs
  time_in_force.fbs
  top_of_book.fbs
  trade.fbs
  trade_summary.fbs
  trade_update.fbs
  trading_status.fbs
)

install(FILES ${SOURCES}
  DESTINATION "${CMAKE_INSTALL_DATADIR}/roq/schema/fbs")

# auto-generated c++ interface

set(FBS_API_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/api.fbs")

set(TARGET_DIR "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/roq/fbs")

file(MAKE_DIRECTORY "${TARGET_DIR}")

set(CPP_API_TARGET "${TARGET_DIR}/api.h")

add_custom_command(
  OUTPUT "${CPP_API_TARGET}"
  COMMAND "${FLATBUFFERS_COMPILER}"
  ARGS --binary
  ARGS --gen-all
  ARGS --filename-suffix ""
  ARGS --cpp --cpp-std c++17
  ARGS "${FBS_API_SOURCE}"
  WORKING_DIRECTORY "${TARGET_DIR}"
  VERBATIM
  DEPENDS "${FBS_API_SOURCE}")

# dependency

add_custom_target(${TARGET_NAME} ALL
  DEPENDS "${CPP_API_TARGET}")

# install

install(FILES "${CPP_API_TARGET}"
  DESTINATION "include/roq/fbs")
