set(TARGET_NAME "${PROJECT_NAME}-test")

set(SOURCES
    alignment.cpp
    fixed_string.cpp
    format.cpp
    metrics.cpp
    span.cpp
    utils.cpp
    main.cpp)

add_executable("${TARGET_NAME}" ${SOURCES})

# dependencies

add_dependencies(${TARGET_NAME} ${PROJECT_NAME}-include-cpp)
add_dependencies(${TARGET_NAME} ${PROJECT_NAME}-include-fbs)

# target

target_link_libraries("${TARGET_NAME}" fmt::fmt gtest_main)

# run

add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")
