set(TARGET_NAME "${PROJECT_NAME}-test")

set(SOURCES
	alignment.cpp
	format.cpp
	metrics.cpp
	span.cpp
	string.cpp
	utils.cpp
	main.cpp
)

add_executable("${TARGET_NAME}" ${SOURCES})

target_link_libraries("${TARGET_NAME}"
  fmt::fmt
  gtest_main
)

# run

add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")
