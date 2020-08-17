set(TARGET_NAME "${PROJECT_NAME}-schema-cpp")

set(TARGET_DIR "${CMAKE_INSTALL_DATADIR}/roq/schema/cpp")

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
    trading_status.json)

# install

install(FILES ${SOURCES} DESTINATION "${TARGET_DIR}")
