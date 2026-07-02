# ==========================================
# @file src/app/CMakeLists.txt
# Build app library
# ==========================================

add_library(${APP_NAME}
    app.cpp
)

target_include_directories(${APP_NAME} PUBLIC
    ${PROJECT_INCLUDE_DIR}
)

target_link_libraries(${APP_NAME} PRIVATE
    ${CORE_NAME}
    async
)

configure_compile_target(${APP_NAME})
configure_link_target(${APP_NAME})
