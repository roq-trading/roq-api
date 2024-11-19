set(TARGET_NAME ${PROJECT_NAME}-test)

set(SOURCES
    alignment.cpp
    compat.cpp
    exceptions.cpp
    format.cpp
    mask.cpp
    rate_limiter.cpp
    request_status.cpp
    side.cpp
    span.cpp
    string.cpp
    support_type.cpp
    traits.cpp
    main.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

add_dependencies(${TARGET_NAME} ${PROJECT_NAME}-include-cpp)

target_link_libraries(${TARGET_NAME} PRIVATE fmt::fmt magic_enum::magic_enum nameof::nameof Catch2::Catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
