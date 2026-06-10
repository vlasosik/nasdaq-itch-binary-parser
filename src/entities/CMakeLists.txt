add_library(entites INTERFACE ${ENTITIES_SOURCES} types.hxx)

target_include_directories(entites INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

