cmake_minimum_required(VERSION 3.5)
project(iontable)

set(SOURCE_FILES
    src/ion_table.c
    src/ion_table.h
    src/stack_memory_manager.c
    src/stack_memory_manager.h
    src/macros.h
    src/tests/unit/test_ion_table.c
    src/tests/unit/run_ion_table.c
    src/ion_table_schema.c
    src/ion_table_schema.h
    src/ion_table_types.h)

add_executable(${PROJECT_NAME}	${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -Wno-vla -Wno-vla-extension -Wno-c99-extensions -Wno-writable-strings -Wno-write-strings")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=c99 -Wno-missing-field-initializers")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g")