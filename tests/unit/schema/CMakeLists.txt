cmake_minimum_required(VERSION 3.5)
project(test_schema)

set(SOURCE_FILES
    ../../../src/schema.h
    ../../../src/schema.c
    ../../../src/types.h
    ../../../src/stack_memory_manager.h
    ../../../src/stack_memory_manager.c
    test_schema.h
    test_schema.c)

if(USE_ARDUINO)
    set(${PROJECT_NAME}_BOARD       ${BOARD})
    set(${PROJECT_NAME}_PROCESSOR   ${PROCESSOR})
    set(${PROJECT_NAME}_MANUAL      ${MANUAL})
    set(${PROJECT_NAME}_PORT        ${PORT})
    set(${PROJECT_NAME}_SERIAL      ${SERIAL})

    set(${PROJECT_NAME}_SKETCH      run_schema.ino)
    set(${PROJECT_NAME}_SRCS        ${SOURCE_FILES})
    set(${PROJECT_NAME}_LIBS        planck_unit)

    generate_arduino_firmware(${PROJECT_NAME})
else()
    add_executable(${PROJECT_NAME}          ${SOURCE_FILES} run_schema.c)

    target_link_libraries(${PROJECT_NAME}   planck_unit)

    # Use cmake -DCOVERAGE_TESTING=ON to include coverage testing information.
    if (CMAKE_COMPILER_IS_GNUCC AND COVERAGE_TESTING)
        set(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
        set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)
    endif()
endif()