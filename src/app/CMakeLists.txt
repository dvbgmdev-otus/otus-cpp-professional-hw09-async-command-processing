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

target_compile_options(${APP_NAME} PRIVATE ${WARN_FLAGS})

if(BUILD_COVERAGE)
    target_compile_options(${APP_NAME} PRIVATE ${COVERAGE_COMPILE_FLAGS})
    if(CMAKE_VERSION VERSION_GREATER "3.13" OR
       CMAKE_VERSION VERSION_EQUAL "3.13")
        target_link_options(${APP_NAME} PRIVATE ${COVERAGE_LINK_FLAGS})
    else()
        set_target_properties(${APP_NAME} PROPERTIES
            LINK_FLAGS "${COVERAGE_LINK_FLAGS}"
        )
    endif()
endif()

if(ENABLE_DEBUG_LOG)
    target_compile_definitions(${APP_NAME} PRIVATE ENABLE_DEBUG_LOG)
endif()
