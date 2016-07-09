cmake_minimum_required(VERSION 3.5)
project(ion_table)

set(SOURCE_FILES
    src/vme.c
    src/vme.h
    src/stack_memory_manager.c
    src/stack_memory_manager.h
    src/macros.h
    src/tests/unit/test_ion_table.c
    src/tests/unit/run_ion_table.c
    src/schema_manager.c
    src/schema_manager.h
    src/types.h)

add_executable(${PROJECT_NAME}	${SOURCE_FILES})

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -g -Wall -Wextra -pedantic -std=c99 -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -std=c++11 -Wno-vla -Wno-vla-extension -Wno-c99-extensions -Wno-writable-strings -Wno-write-strings")