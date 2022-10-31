set(TARGET_NAME "${PROJECT_NAME}-include-cpp")

set(TARGET_DIR "${CMAKE_INSTALL_INCLUDEDIR}/roq")

set(SOURCES
    batch_begin.json
    batch_end.json
    buffer_capacity.json
    cancel_all_orders.json
    cancel_order.json
    category.json
    connected.json
    connection_status.json
    create_order.json
    custom_metrics.json
    custom_metrics_update.json
    decimals.json
    disconnected.json
    download_begin.json
    download_end.json
    encoding.json
    error.json
    execution_instruction.json
    external_latency.json
    fill.json
    funds_update.json
    gateway_settings.json
    gateway_status.json
    layer.json
    liquidity.json
    market_by_order_update.json
    market_by_price_update.json
    market_status.json
    mbo_update.json
    mbp_update.json
    measurement.json
    message_info.json
    modify_order.json
    option_type.json
    order_ack.json
    order_cancel_policy.json
    order_management.json
    order_status.json
    order_type.json
    order_update.json
    origin.json
    parameter.json
    parameters_update.json
    position_effect.json
    position_update.json
    priority.json
    protocol.json
    rate_limit_trigger.json
    rate_limit_type.json
    reference_data.json
    request_id_type.json
    request_status.json
    request_type.json
    security_type.json
    side.json
    start.json
    statistics.json
    statistics_type.json
    statistics_update.json
    stop.json
    stream_status.json
    support_type.json
    time_in_force.json
    timer.json
    top_of_book.json
    trade.json
    trade_summary.json
    trade_update.json
    trading_status.json
    transport.json
    update_action.json
    update_type.json)

# configure

set(API_HPP "${CMAKE_BINARY_DIR}/${TARGET_DIR}/api.hpp")

configure_file("api.hpp.in" "${API_HPP}" @ONLY)

# auto-generate

unset(AUTOGEN_HEADERS)

foreach(path ${SOURCES})
  set(source "${SCHEMA_DIR}/cpp/${path}")
  get_filename_component(name ${path} NAME_WE)
  set(target "${CMAKE_BINARY_DIR}/${TARGET_DIR}/${name}.hpp")
  add_custom_command(
    OUTPUT "${target}"
    COMMAND ${PYTHON_EXECUTABLE} "${AUTOGEN_PY}" --namespace "roq" --file-type "hpp" "${source}" >
            "${target}"
    COMMAND ${CLANG_FORMAT} -i "${target}"
    VERBATIM
    DEPENDS "${PYTHON_EXECUTABLE}" "${AUTOGEN_PY}" "${source}")
  list(APPEND AUTOGEN_HEADERS "${target}")
endforeach()

# target

add_custom_target("${TARGET_NAME}" ALL DEPENDS ${AUTOGEN_HEADERS} "${GITIGNORE}")

# install

install(FILES ${AUTOGEN_HEADERS} "${API_HPP}" DESTINATION "${TARGET_DIR}")
