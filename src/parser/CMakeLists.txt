file(GLOB_RECURSE PARSER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(parcer ${PARSER_SOURCES})

target_include_directories(parcer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(parcer PRIVATE quill::quill entites)

