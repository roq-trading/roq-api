set(TARGET_NAME "${PROJECT_NAME}-schema-cpp")

set(TARGET_DIR "${CMAKE_INSTALL_DATADIR}/roq/schema/cpp")

set(SOURCES
    batch_begin.json
    batch_end.json
    buffer_capacity.json
    cancel_all_orders.json
    cancel_order.json
    connected.json
    connection_status.json
    create_order.json
    custom_metrics.json
    custom_metrics_update.json
    disconnected.json
    download_begin.json
    download_end.json
    encoding.json
    error.json
    execution_instruction.json
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
    parameter_update.json
    position_effect.json
    position_update.json
    priority.json
    protocol.json
    rate_limit_trigger.json
    rate_limit_type.json
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

# install

install(FILES ${SOURCES} DESTINATION "${TARGET_DIR}")
