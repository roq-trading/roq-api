set(TARGET_NAME ${PROJECT_NAME}-schema-java)

set(TARGET_DIR ${CMAKE_INSTALL_DATADIR}/roq/schema/java)

set(SOURCES
    BatchBegin.json
    BatchEnd.json
    BufferCapacity.json
    Category.json
    Connected.json
    ConnectionStatus.json
    Disconnected.json
    DownloadBegin.json
    DownloadEnd.json
    Encoding.json
    Error.json
    ExecutionInstruction.json
    ExternalLatency.json
    Filter.json
    GatewaySettings.json
    GatewayStatus.json
    Layer.json
    Liquidity.json
    MarginMode.json
    MarketByOrderUpdate.json
    MarketByPriceUpdate.json
    MarketStatus.json
    MBOUpdate.json
    MBPUpdate.json
    MessageInfo.json
    OptionType.json
    OrderCancelPolicy.json
    OrderManagement.json
    OrderStatus.json
    OrderType.json
    Origin.json
    PositionEffect.json
    Precision.json
    Priority.json
    Protocol.json
    RateLimit.json
    RateLimitsUpdate.json
    RateLimitTrigger.json
    RateLimitType.json
    Ready.json
    ReferenceData.json
    RequestIdType.json
    RequestStatus.json
    RequestType.json
    RouteRequestStatus.json
    SecurityType.json
    Side.json
    Start.json
    Statistics.json
    StatisticsType.json
    StatisticsUpdate.json
    Stop.json
    StreamStatus.json
    SupportType.json
    TimeInForce.json
    TopOfBook.json
    Trade.json
    TradeSummary.json
    TradingStatus.json
    Transport.json
    UpdateAction.json
    UpdateReason.json
    UpdateType.json)

# install

install(FILES ${SOURCES} DESTINATION ${TARGET_DIR})
