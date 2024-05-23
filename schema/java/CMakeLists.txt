set(TARGET_NAME ${PROJECT_NAME}-schema-java)

set(TARGET_DIR ${CMAKE_INSTALL_DATADIR}/roq/schema/java)

set(SOURCES
    BatchBegin.json
    BatchEnd.json
    Connected.json
    ConnectionStatus.json
    Disconnected.json
    DownloadBegin.json
    DownloadEnd.json
    Encoding.json
    MessageInfo.json
    OrderCancelPolicy.json
    Priority.json
    Protocol.json
    Ready.json
    Side.json
    Start.json
    Stop.json
    StreamStatus.json
    SupportType.json
    Transport.json)

# install

install(FILES ${SOURCES} DESTINATION ${TARGET_DIR})
