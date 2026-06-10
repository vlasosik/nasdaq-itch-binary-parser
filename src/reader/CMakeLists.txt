file(GLOB_RECURSE READER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(reader ${READER_SOURCES})

target_include_directories(reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(reader PRIVATE ZLIB::ZLIB)

