set(TARGET_NAME ${PROJECT_NAME}-test)

set(SOURCES
    alignment.cpp
    compare.cpp
    compat.cpp
    datetime.cpp
    debug.cpp
    exceptions.cpp
    format.cpp
    json.cpp
    mask.cpp
    rate_limiter.cpp
    request_status.cpp
    side.cpp
    span.cpp
    string.cpp
    support_type.cpp
    traits.cpp
    update.cpp
    utils.cpp
    uuid.cpp
    main.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

add_dependencies(${TARGET_NAME} ${PROJECT_NAME}-include-cpp)

target_link_libraries(${TARGET_NAME} PRIVATE absl::hash Catch2::Catch2 fmt::fmt)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
