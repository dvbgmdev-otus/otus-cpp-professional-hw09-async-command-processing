# ==========================================
# @file src/main/CMakeLists.txt
# Build bulk executable
# ==========================================

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${APP_NAME}
)

configure_compile_target(${PROJECT_NAME})
configure_link_target(${PROJECT_NAME})

list(APPEND COVERAGE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
set(COVERAGE_SRC ${COVERAGE_SRC} PARENT_SCOPE)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
