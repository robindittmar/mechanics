
file (GLOB XORSTRINGS_SOURCE_FILES "*.cpp")
file (GLOB XORSTRINGS_HEADER_FILES "*.h")

add_executable(
        XorStrings
        ${XORSTRINGS_SOURCE_FILES}
        ${XORSTRINGS_HEADER_FILES}
        # Add XorString & murmurhas from mechanics project
        ../../mechanics/src/XorString.h
        ../../mechanics/src/XorString.cpp
        ../../mechanics/src/murmurhash.h
        ../../mechanics/src/murmurhash.cpp
)

