
set (VERSION_MAJOR 1 CACHE STRING "Major version number")
set (VERSION_MINOR 0 CACHE STRING "Minor version number")
set (VERSION_PATCH 0 CACHE STRING "Patch version number")
set (VERSION_COMMIT git CACHE STRING "Git commit hash, will be fetched automatically if value is \"git\"")
set (VERSION_BRANCH git CACHE STRING "Git branch, will be fetched automatically if value is \"git\"")

if (VERSION_COMMIT STREQUAL "git")
    execute_process(
            COMMAND git log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE VERSION_COMMIT
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif(VERSION_COMMIT STREQUAL "git")

if (VERSION_BRANCH STREQUAL "git")
    execute_process(
            COMMAND git rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE VERSION_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif(VERSION_BRANCH STREQUAL "git")

file (GLOB MECHANICS_SOURCE_FILES "*.cpp")
file (GLOB MECHANICS_HEADER_FILES "*.h")

file (GLOB MECHANICS_CORE_SOURCE_FILES "core/*.cpp")
file (GLOB MECHANICS_CORE_HEADER_FILES "core/*.h")

file (GLOB MECHANICS_FEATURES_SOURCE_FILES "features/*.cpp")
file (GLOB MECHANICS_FEATURES_HEADER_FILES "features/*.h")

file (GLOB MECHANICS_GUI_SOURCE_FILES "gui/*.cpp")
file (GLOB MECHANICS_GUI_HEADER_FILES "gui/*.h")

file (GLOB MECHANICS_HOOKS_SOURCE_FILES "hooks/*.cpp")
file (GLOB MECHANICS_HOOKS_HEADER_FILES "hooks/*.h")

file (GLOB MECHANICS_PROXIES_SOURCE_FILES "proxies/*.cpp")
file (GLOB MECHANICS_PROXIES_HEADER_FILES "proxies/*.h")

file (GLOB MECHANICS_SOURCESDK_SOURCE_FILES "source_sdk/*.cpp")
file (GLOB MECHANICS_SOURCESDK_HEADER_FILES "source_sdk/*.h")

file (GLOB MECHANICS_THIRDPARTY_SOURCE_FILES "third_party/*/*.cpp")
file (GLOB MECHANICS_THIRDPARTY_HEADER_FILES "third_party/*/*.h")

file (GLOB MECHANICS_UTILS_SOURCE_FILES "utils/*.cpp")
file (GLOB MECHANICS_UTILS_HEADER_FILES "utils/*.h")

configure_file(
        version.h.in
        "${PROJECT_BINARY_DIR}/include/version.h"
)

include_directories(${CMAKE_BINARY_DIR}/include)

add_library(
        mechanics
        SHARED
        ${MECHANICS_SOURCE_FILES}
        ${MECHANICS_HEADER_FILES}
        ${MECHANICS_CORE_SOURCE_FILES}
        ${MECHANICS_CORE_HEADER_FILES}
        ${MECHANICS_FEATURES_SOURCE_FILES}
        ${MECHANICS_FEATURES_HEADER_FILES}
        ${MECHANICS_GUI_SOURCE_FILES}
        ${MECHANICS_GUI_HEADER_FILES}
        ${MECHANICS_HOOKS_SOURCE_FILES}
        ${MECHANICS_HOOKS_HEADER_FILES}
        ${MECHANICS_PROXIES_SOURCE_FILES}
        ${MECHANICS_PROXIES_HEADER_FILES}
        ${MECHANICS_SOURCESDK_SOURCE_FILES}
        ${MECHANICS_SOURCESDK_HEADER_FILES}
        ${MECHANICS_THIRDPARTY_SOURCE_FILES}
        ${MECHANICS_THIRDPARTY_HEADER_FILES}
        ${MECHANICS_UTILS_SOURCE_FILES}
        ${MECHANICS_UTILS_HEADER_FILES}
        "${PROJECT_BINARY_DIR}/include/version.h"
)

