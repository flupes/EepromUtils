# Build the test programs

set(LIBS eepromUtils arduinolibs corearduino)

# Create the executables
add_program(showEeprom ${LIBS})
add_program(enduranceEepromClear ${LIBS})
add_program(enduranceEepromTest ${LIBS})
add_program(eepromRingBufferClear ${LIBS})
add_program(eepromRingBufferTest ${LIBS})
add_program(timeRingBufferClear ${LIBS})
add_program(timeRingBufferTest ${LIBS})
add_program(eepromSpeedTest ${LIBS})
add_program(clearEeprom ${LIBS})
