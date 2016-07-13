cmake_minimum_required(VERSION 3.5)
project(vme)

set(SOURCE_FILES
    vme.c
    vme.h
    stack_memory_manager.c
    stack_memory_manager.h
    macros.h
    schema.c
    schema.h
    types.h
    utils.h
    utils.c
    query_serializer.h
    query_serializer.c)

set(IONDB_SOURCE_FILES
    iondb/src/kv_system.h
    iondb/src/kv_io.h
    iondb/src/kv_io.c
    iondb/src/dictionary/dictionary.h
    iondb/src/dictionary/dictionary.c
    iondb/src/dictionary/dictionary_types.h
    iondb/src/dictionary/ion_master_table.h
    iondb/src/dictionary/ion_master_table.c
    iondb/src/dictionary/bpp_tree/bpp_tree.h
    iondb/src/dictionary/bpp_tree/bpp_tree.c
    iondb/src/dictionary/bpp_tree/bpp_tree_handler.h
    iondb/src/dictionary/bpp_tree/bpp_tree_handler.c utils.h utils.c)

if(USE_ARDUINO)
    set(${PROJECT_NAME}_BOARD       ${BOARD})
    set(${PROJECT_NAME}_PROCESSOR   ${PROCESSOR})
    set(${PROJECT_NAME}_MANUAL      ${MANUAL})

    set(${PROJECT_NAME}_SRCS
        ${SOURCE_FILES}
        ${IONDB_SOURCE_FILES}
        serial/printf_redirect.h
        serial/serial_c_iface.h
        serial/serial_c_iface.cpp
        file/kv_stdio_intercept.h
        file/SD_stdio_c_iface.h
        file/SD_stdio_c_iface.cpp)

    generate_arduino_library(${PROJECT_NAME})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${IONDB_SOURCE_FILES})

    # Required on Unix OS family to be able to be linked into shared libraries.
    set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()