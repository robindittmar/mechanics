
file (GLOB MECHANICS_SOURCE_FILES "*.cpp")
file (GLOB MECHANICS_HEADER_FILES "*.h")

add_library(
        mechanics
        SHARED
        ${MECHANICS_SOURCE_FILES}
        ${MECHANICS_HEADER_FILES}
)

