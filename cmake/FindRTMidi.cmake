find_path(RTMidi_INCLUDE_DIRS RtMidi.h
    DOC
    "Found RTMidi include directory"
    ENV
        RTMidi_ROOT
    PATH_SUFFIXES
        include/rtmidi
)
find_library(RTMidi_LIBRARIES
    NAMES librtmidi.so rtmidi
    DOC "Found RTMidi library path"
    ENV
        RTMidi_ROOT
    PATH_SUFFIXES
        lib
        lib64
)
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RTMidi
    FOUND_VAR
        RTMidi_FOUND
    REQUIRED_VARS
        RTMidi_LIBRARIES
        RTMidi_INCLUDE_DIRS
)
if (RTMidi_FOUND AND NOT TARGET RTMidi::RTMidi)
    add_library(RTMidi::RTMidi STATIC IMPORTED)
    set_target_properties(RTMidi::RTMidi PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${RTMidi_INCLUDE_DIRS})
    set_target_properties(RTMidi::RTMidi PROPERTIES IMPORTED_LOCATION ${RTMidi_LIBRARIES})
endif ()
