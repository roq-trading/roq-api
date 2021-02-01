set(TARGET_NAME "${PROJECT_NAME}-schema-fbs")

set(TARGET_DIR "${CMAKE_INSTALL_DATADIR}/roq/schema/fbs")

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
    gateway_settings.fbs
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
    trading_status.fbs)

# install

install(FILES ${SOURCES} DESTINATION "${TARGET_DIR}")
