set(TARGET_NAME ${PROJECT_NAME}-include-cpp)

set(TARGET_DIR ${CMAKE_INSTALL_INCLUDEDIR}/roq)

# schema

# XXX FIXME
set(SCHEMA_ROQ
    add_routes.json
    batch_begin.json
    batch_end.json
    buffer_capacity.json
    cancel_all_orders_ack.json
    cancel_all_orders.json
    cancel_order.json
    category.json
    connected.json
    connection_status.json
    create_order.json
    custom_matrix.json
    custom_matrix_update.json
    custom_metrics.json
    custom_metrics_update.json
    disconnected.json
    download_begin.json
    download_end.json
    encoding.json
    error.json
    execution_instruction.json
    external_latency.json
    fill.json
    filter.json
    funds_update.json
    gateway_settings.json
    gateway_status.json
    layer.json
    liquidity.json
    margin_mode.json
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
    portfolio.json
    portfolio_update.json
    position_effect.json
    position.json
    position_update.json
    precision.json
    priority.json
    protocol.json
    quality_of_service.json
    rate_limit.json
    rate_limits_update.json
    rate_limit_trigger.json
    rate_limit_type.json
    ready.json
    reference_data.json
    remove_routes.json
    request_id_type.json
    request_status.json
    request_type.json
    risk_limit.json
    risk_limits.json
    risk_limits_update.json
    route_ack.json
    route.json
    route_request_status.json
    routing.json
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
    update_reason.json
    update_type.json)

set(SOURCES ${SCHEMA_ROQ})

# configure

set(API_HPP ${CMAKE_BINARY_DIR}/${TARGET_DIR}/api.hpp)

configure_file("api.hpp.in" ${API_HPP} @ONLY)

# auto-generate

unset(AUTOGEN_HEADERS)

foreach(path ${SOURCES})
  get_filename_component(name ${path} NAME_WE)
  set(target ${CMAKE_BINARY_DIR}/${TARGET_DIR}/${name}.hpp)
  set(link ${SCHEMA_LINK_DIR}/roq/${path})
  set_property(
    DIRECTORY
    APPEND
    PROPERTY CMAKE_CONFIGURE_DEPENDS ${link})
  add_custom_command(
    OUTPUT ${target}
    COMMAND ${ROQ_AUTOGEN} --schema_dir ${SCHEMA_DIR} --template_dir ${TEMPLATE_DIR} --input_path
            ${link} --language "cpp" --template_type "api" --file_type "hpp" > ${target}
    COMMAND ${CLANG_FORMAT} -i ${target}
    VERBATIM
    DEPENDS ${ROQ_AUTOGEN} ${link})
  list(APPEND AUTOGEN_HEADERS ${target})
endforeach()

# target

add_custom_target(${TARGET_NAME} ALL DEPENDS ${AUTOGEN_HEADERS} ${GITIGNORE})

# install

install(FILES ${AUTOGEN_HEADERS} ${API_HPP} DESTINATION ${TARGET_DIR})
