# Build the test programs

set(LIBS arduinoutils arduinolibs corearduino)

# Create the executables
add_program(showEeprom ${LIBS})
add_program(enduranceEepromClear ${LIBS})
add_program(enduranceEepromTest ${LIBS})
add_program(eepromRingBufferClear ${LIBS})
add_program(eepromRingBufferTest ${LIBS})
