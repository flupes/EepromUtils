# Define the local library
add_library(eepromUtils
    SafeEeprom.cpp
    EnduranceEeprom.cpp
    EepromRingBuffer.cpp
    TimePermRingBuffer.cpp
)

# Where to find the includes
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${libspath}/Wire )

add_subdirectory ( tests )

