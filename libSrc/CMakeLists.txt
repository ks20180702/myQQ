include_directories(../include)

aux_source_directory(. SRC_LISTS)

add_library(lib_shared SHARED ${SRC_LISTS})
add_library(lib_static STATIC ${SRC_LISTS})

set_target_properties(lib_shared PROPERTIES OUTPUT_NAME "funShared")
set_target_properties(lib_static PROPERTIES OUTPUT_NAME "funStatic")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)