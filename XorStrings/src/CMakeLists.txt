
file (GLOB XORSTRINGS_SOURCE_FILES "*.cpp")
file (GLOB XORSTRINGS_HEADER_FILES "*.h")

add_executable(
        XorStrings
        ${XORSTRINGS_SOURCE_FILES}
        ${XORSTRINGS_HEADER_FILES}
        # Add XorString & murmurhas from mechanics project
        ../../mechanics/src/utils/XorString.h
        ../../mechanics/src/utils/XorString.cpp
        ../../mechanics/src/third_party/murmurhash/murmurhash.h
        ../../mechanics/src/third_party/murmurhash/murmurhash.cpp
)

