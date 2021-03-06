cmake_minimum_required(VERSION 3.5)
project(ion_table)

# Use cmake -DUSE_ARDUINO=ON <project_folder> to compile to Arduino.
if(USE_ARDUINO)
    message("Linking against Arduino...")
    set(CMAKE_TOOLCHAIN_FILE    ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
    set(PROJECT_NAME            IonDB)
    project(IonDB               C CXX)

    # User options
    set(BOARD ${BOARD})

    if(NOT PROCESSOR)
        set(PROCESSOR)
    endif()

    set(PORT ${PORT})

    if(NOT BAUD_RATE)
        set(BAUD_RATE 9600)
    endif()

    if(NOT MANUAL)
        set(MANUAL FALSE)
    endif()

    if(NOT FORMATTED_SD_CARD_CS)
        set(FORMATTED_SD_CARD_CS 4)
    endif()

    message(${BAUD_RATE})
    add_definitions("-D BAUD_RATE=${BAUD_RATE} -D SD_CS_PIN=${FORMATTED_SD_CARD_CS}")

    if(WIN32)
        set(SERIAL putty -serial ${PORT} -sercfg ${BAUD_RATE})
    else()
        set(SERIAL screen ${PORT} ${BAUD_RATE})
    endif()

    print_board_list()
    print_programmer_list()
else()
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -g -Wall -Wextra -pedantic -std=c99 -Wno-missing-field-initializers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -std=c++11 -Wno-vla -Wno-vla-extension -Wno-c99-extensions -Wno-writable-strings -Wno-write-strings")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

add_subdirectory(src)

add_subdirectory(tests)
add_subdirectory(tests/unit/vme)
add_subdirectory(tests/unit/schema)