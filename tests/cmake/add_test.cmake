# @file tests/cmake/add_test.cmake
# ===== Функция добавления тестов =====
function(add_unit_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})

    target_link_libraries(${TEST_NAME} PRIVATE
        ${CORE_NAME}
        GTest::gtest_main
    )

    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_BIN_DIR}
    )

    configure_compile_target(${TEST_NAME})
    configure_link_target(${TEST_NAME})

    # add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    gtest_discover_tests(${TEST_NAME})

endfunction()
