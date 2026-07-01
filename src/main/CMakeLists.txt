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

target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_FLAGS})

if(ENABLE_DEBUG_LOG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_DEBUG_LOG)
endif()

list(APPEND COVERAGE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
set(COVERAGE_SRC ${COVERAGE_SRC} PARENT_SCOPE)

if(BUILD_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE ${COVERAGE_COMPILE_FLAGS})
    if(CMAKE_VERSION VERSION_GREATER "3.13" OR
       CMAKE_VERSION VERSION_EQUAL "3.13")
        target_link_options(${PROJECT_NAME} PRIVATE ${COVERAGE_LINK_FLAGS})
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS "${COVERAGE_LINK_FLAGS}"
        )
    endif()
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
