set(TARGET_NAME ${PROJECT_NAME}-test)

set(SOURCES
    alignment.cpp
    compat.cpp
    exceptions.cpp
    format.cpp
    mask.cpp
    side.cpp
    span.cpp
    string.cpp
    support_type.cpp
    main.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

add_dependencies(${TARGET_NAME} ${PROJECT_NAME}-include-cpp)

target_link_libraries(${TARGET_NAME} PRIVATE fmt::fmt magic_enum::magic_enum nameof::nameof Catch2::Catch2)

if(ROQ_BUILD_TYPE STREQUAL "Release")
  set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
