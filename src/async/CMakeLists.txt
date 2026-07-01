# ==========================================
# @file src/async/CMakeLists.txt
# Build async library
# ==========================================

set(ASYNC_NAME async)

add_library(${ASYNC_NAME} SHARED
    async.cpp
    async_context.cpp
    async_runtime.cpp
)

target_include_directories(${ASYNC_NAME} PUBLIC
    ${PROJECT_INCLUDE_DIR}
)

target_link_libraries(${ASYNC_NAME} PRIVATE
    ${CORE_NAME}
)

target_compile_options(${ASYNC_NAME} PRIVATE ${WARN_FLAGS})

if(BUILD_COVERAGE)
    target_compile_options(${ASYNC_NAME} PRIVATE ${COVERAGE_COMPILE_FLAGS})
    if(CMAKE_VERSION VERSION_GREATER "3.13" OR
       CMAKE_VERSION VERSION_EQUAL "3.13")
        target_link_options(${ASYNC_NAME} PRIVATE ${COVERAGE_LINK_FLAGS})
    else()
        set_target_properties(${ASYNC_NAME} PROPERTIES
            LINK_FLAGS "${COVERAGE_LINK_FLAGS}"
        )
    endif()
endif()

if(ENABLE_DEBUG_LOG)
    target_compile_definitions(${ASYNC_NAME} PRIVATE ENABLE_DEBUG_LOG)
endif()

install(TARGETS ${ASYNC_NAME} LIBRARY DESTINATION lib)
install(FILES async.h DESTINATION include)
