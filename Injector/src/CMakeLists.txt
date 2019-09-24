
file (GLOB INJECTOR_SOURCE_FILES "*.cpp")
file (GLOB INJECTOR_HEADER_FILES "*.h")

add_executable(
        Injector
        ${INJECTOR_SOURCE_FILES}
        ${INJECTOR_HEADER_FILES}
)

