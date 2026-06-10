add_library(exceptions INTERFACE ${EXCEPTIONS_SOURCES} parse_error.hxx)

target_include_directories(exceptions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(exceptions INTERFACE fmt::fmt)
